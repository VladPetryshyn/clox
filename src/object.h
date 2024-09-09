#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"


#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->chars)
#define AS_FUNCTION(value)     ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value) \
    (((ObjNative*)AS_OBJ(value))->function)
#define OBJ_TYPE(value)        (AS_OBJ(value)->type)
#define IS_FUNCTION(value)     isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)       isObjType(value, OBJ_NATIVE)
#define IS_STRING(value)       isObjType(value, OBJ_STRING)

typedef enum {
  OBJ_STRING,
  OBJ_FUNCTION,
  OBJ_NATIVE,
} ObjType;

struct Obj {
  ObjType type;
  // linked list next
  struct Obj* next;
};

// functions are first class objects in lox
typedef struct {
  Obj obj;
  // number of parameters function shares
  int arity;
  Chunk chunk;
  // func name
  ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

// Native Functions differ from the lox functions
typedef struct {
  Obj obj;
  NativeFn function;
} ObjNative;

struct ObjString {
  // struct inheritance as in go.
  Obj obj;
  int length;
  char* chars;
  uint32_t hash;
};

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
ObjFunction* newFunction();
ObjNative* newNative(NativeFn function);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif

