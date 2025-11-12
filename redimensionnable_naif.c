#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tracked_alloc.h"

typedef struct {
    uint8_t *data;    
    size_t size;      
    size_t capacity;  
} DynArrayU8;

int da_init(DynArrayU8 *da, size_t initial_capacity) {
    if (!da) return -1;
    da->size = 0;
    da->capacity = (initial_capacity > 0) ? initial_capacity : 1;
    da->data = tracked_malloc(da->capacity * sizeof(uint8_t));
    if (!da->data) return -1;
    return 0;
}

void da_free(DynArrayU8 *da) {
    if (!da) return;
    tracked_free(da->data, da->capacity);
    da->data = NULL;
    da->size = 0;
    da->capacity = 0;
}

static int da_grow(DynArrayU8 *da) {
    size_t new_capacity = da->capacity * 2;
    
    uint8_t *new_data = tracked_realloc(da->data, da->capacity, new_capacity * sizeof(uint8_t));
    if (!new_data) return -1;
    da->data = new_data;
    da->capacity = new_capacity;
    return 0;
}

int da_push_back(DynArrayU8 *da, uint8_t value) {
    if (!da) return -1;
    if (da->size == da->capacity) {
        if (da_grow(da) != 0) return -1;
    }
    da->data[da->size++] = value;
    return 0;
}

int da_pop_back(DynArrayU8 *da, uint8_t *out) {
    if (!da || da->size == 0) return -1;
    da->size--;
    if (out) *out = da->data[da->size];
    return 0;
}

int da_get(const DynArrayU8 *da, size_t index, uint8_t *out) {
    if (!da || index >= da->size || !out) return -1;
    *out = da->data[index];
    return 0;
}

int da_set(DynArrayU8 *da, size_t index, uint8_t value) {
    if (!da || index >= da->size) return -1;
    da->data[index] = value;
    return 0;
}

size_t da_size(const DynArrayU8 *da) {
    return da ? da->size : 0;
}

size_t da_capacity(const DynArrayU8 *da) {
    return da ? da->capacity : 0;
}

