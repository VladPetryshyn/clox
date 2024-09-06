#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct {
  ObjString* key;
  Value value;
} Entry;

typedef struct {
  // key/value pairs in the array
  int count;
  // allocated size of the array.
  int capacity;
  // ratio of these 2 above is our load factor

  Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
ObjString* tableFindString(Table* table, const char* chars,
                           int length, uint32_t hash);
bool tableSet(Table* table, ObjString* key, Value value);
// copy table
void tableAddAll(Table* from, Table* to);
// retrieve value from the table.
bool tableGet(Table* table, ObjString* key, Value* value);
bool tableDelete(Table* table, ObjString* key);

#endif
