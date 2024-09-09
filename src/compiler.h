#ifndef clox_compiler_h
#define clox_compiler_h
// local variables limit
#define UINT8_COUNT (UINT8_MAX + 1)

#include "chunk.h"
#include "scanner.h"
#include "object.h"

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} Precedence;
typedef void (*ParseFn)(bool canAssign);


typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

typedef struct {
  // variable name
  Token name;
  // we store depth, to know which locals to discard, when a block ends
  int depth;
} Local;

typedef enum {
  TYPE_FUNCTION,
  TYPE_SCRIPT
} FunctionType;

typedef struct Compiler {
  ObjFunction* function;
  FunctionType type;
  struct Compiler* enclosing;
  Local locals[UINT8_COUNT];
  int localCount;
  int scopeDepth;
} Compiler;

ObjFunction* compile(const char* source);
static void advance();
static void function(FunctionType type);
#endif
