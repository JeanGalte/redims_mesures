#ifndef TRACKED_MALLOC_H
#define TRACKED_MALLOC_H

#include <stdlib.h>
#include <stddef.h>

void* tracked_malloc(size_t size);
void* tracked_calloc(int count, size_t size); 
void  tracked_free(void* ptr, size_t size);
void* tracked_realloc(void* ptr, size_t old_size, size_t new_size); 
size_t get_total_allocated(void);

#endif
