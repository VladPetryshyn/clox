#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

/*
* When we have a full-featured VM, this instruction will mean “return from the current function”.
*/
typedef enum {
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_RETURN,
  OP_NEGATE,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_ADD,
  OP_NOT,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
} OpCode;

// wrapper for the dynamic array
// we keep count, and capacity to expand it, when we hit the limit
typedef struct {
	int count;
	int capacity;
	// array
	uint8_t* code;
	ValueArray constants;
	// chunk lines for the  runtime errors
	int* lines;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);
int writeConstant(Chunk* chunk, Value value, int line);

#endif

