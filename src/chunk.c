#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

void initChunk(Chunk *chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
	if (chunk->capacity < chunk->count+1) {
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
		chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	chunk->lines[chunk->count] = line;
	chunk->count++;
}

int writeConstant(Chunk* chunk, Value value, int line) {
	int index = addConstant(chunk, value);
	if (index <= 0xff) {
		writeChunk(chunk, OP_CONSTANT, line);
		writeChunk(chunk, index, line);
	} else {
		// I know nothing about bit shifting
		writeChunk(chunk, OP_CONSTANT_LONG, line);
		writeChunk(chunk, index >> 16, line);
		writeChunk(chunk, (index >> 8) & 0xff, line);
		writeChunk(chunk, index & 0xff, line);
	}
	return index;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int addConstant(Chunk *chunk, Value value) {
	push(value);
	writeValueArray(&chunk->constants, value);
	pop();
	return chunk->constants.count - 1;
}
