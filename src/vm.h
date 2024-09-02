#ifndef clox_vm_h
#define clox_vm_h
#include "value.h"

#include "chunk.h"


#define STACK_MAX 256
typedef struct {
  Chunk* chunk;
  // instruction pointer that will be executed next;
  uint8_t* ip;
  Value stack[STACK_MAX];
  // points past the last element;
  Value* stackTop;
  // linked list head
  Obj* objects;
} VM;

extern VM vm;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

bool valuesEqual(Value a, Value b);
InterpretResult interpret(const char* source);
void initVM();
void freeVM();
void push(Value value);
Value pop();

#endif

