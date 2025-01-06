#include "talloc.h"
#include <stdio.h>
#include <stdbool.h>

#define CAPACITY 1024

typedef struct FreeNode {
    struct FreeNode *next;
    size_t size;
} FreeNode;

static char heap[CAPACITY] = {0};

static FreeNode *freelist = NULL;
static bool empty = false;

static void init_freelist() {
    freelist = (FreeNode*) heap;
    freelist -> next = NULL;
    freelist -> size = CAPACITY;
}

static void update_freelist(FreeNode* previous, FreeNode* current, size_t size) {
    if (current -> size == size) {
        if (!previous) {
            freelist = current -> next;
            empty = !(current -> next);
        } else {
            previous -> next = current -> next;
        }
    } else {
        FreeNode* newNode = (FreeNode*) ((char*) current + size);
        newNode -> next = current -> next;
        newNode -> size = current -> size - size;
        if (!previous)
            freelist = newNode;
        else
            previous -> next = newNode;
    }
}

void* talloc(size_t size) {
    if (!size)
        return NULL;

    if (size < sizeof(FreeNode))
        size = sizeof(FreeNode);

    size += sizeof(size_t);

    size += ((sizeof(FreeNode) - (size % sizeof(FreeNode))) % sizeof(FreeNode));

    if (!empty && !freelist)
        init_freelist();
    
    // Find first big enough chunk
    FreeNode* previous = NULL;
    FreeNode* current = freelist;
    while (current && current -> size < size) {
        previous = current;
        current = current -> next;
    }

    // If no big enough space found retun NULL
    if (!current)
        return NULL;

    update_freelist(previous, current, size);

    *(size_t*)current = size;

    return (void*) ((char*) current + sizeof(size_t));
}

static void merge_blocks(FreeNode* previous, FreeNode* block, FreeNode* next) {
    bool merged = false;
    if (previous && (((char*)block - (char*)previous) == previous -> size)) {
        previous -> next = block -> next;
        previous -> size += block -> size;
        merged = true;
    }

    if (previous && next && merged
        && (((char*)next - (char*)previous) == previous -> size)) {
        previous -> next = next -> next;
        previous -> size += next -> size; 
    } else if (next && (((char*)next - (char*)block) == block -> size)) {
        block -> next = next -> next;
        block -> size += next -> size;
    }
}

void tfree(void* ptr) {
    if (!ptr)
        return;

    empty = false;
    
    void* blockStart = (void*) ((char*) ptr - sizeof(size_t));

    size_t size = *(size_t*)blockStart;

    FreeNode* block = (FreeNode*) blockStart;
    block -> size = size;
    
    FreeNode* current = freelist;
    FreeNode* previous = NULL;
    while (current && current < block) {
        previous = current;
        current = current -> next;
    }

    block -> next = current;
    if (!previous)
        freelist = block;
    else
        previous -> next = block;

    merge_blocks(previous, block, current);
}