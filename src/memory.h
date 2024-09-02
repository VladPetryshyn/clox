#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// this is a macro, that calculates new capacity based on a given current capacity.
// we grow capacity by a factor of 2
// if the capacity is 0, we start at 8
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

// single function responsible for all dynamic memory management, allocating, freeing,
// changing size of an existing allocation
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void freeObjects();

#endif
