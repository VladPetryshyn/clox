#ifndef clox_vm_h
#define clox_vm_h
#include "compiler.h"
#include "value.h"
#include "table.h"

#include "chunk.h"
#include "object.h"


#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
  ObjFunction* function;
  ObjClosure* closure;
  uint8_t* ip;
  Value* slots;
} CallFrame;

typedef struct {
  // function call frames
  CallFrame frames[FRAMES_MAX];
  int frameCount;

  Value stack[STACK_MAX];
  // points past the last element;
  Value* stackTop;
  // linked list head
  Obj* objects;
  // used for the strings comparison
  Table strings;
  // used to store global variables
  Table globals;
  ObjUpvalue* openUpvalues;
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

