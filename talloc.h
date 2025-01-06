#ifndef TALLOC_H
#define TALLOC_H

#include <stddef.h>

// Allocate memory from a fixed-size heap
void* talloc(size_t size);

// Free allocated memory back to the heap
void tfree(void* ptr);

#endif