#include "tracked_alloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static size_t total_allocated = 0;

void* tracked_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) total_allocated += size;
    return ptr;
}

void* tracked_realloc(void* ptr, size_t old_size, size_t new_size) {
    void* new_ptr = realloc(ptr, new_size);
    if (new_ptr) {
        if (new_size > old_size)
            total_allocated += new_size - old_size;
        else
            total_allocated -= old_size - new_size;
    }
    return new_ptr;
}

void tracked_free(void* ptr, size_t size) {
    if (ptr) {
        total_allocated -= size;
        free(ptr);
    }
}

void* tracked_calloc(int count, size_t size) {
  int total = count * size;
  void *ptr = malloc(total); 
  if (!ptr) return NULL;
  memset(ptr, 0, total);
  return ptr; 
}

size_t get_total_allocated(void) {
    return total_allocated;
}
