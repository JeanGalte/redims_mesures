#ifndef DYNARRAY_U8_H
#define DYNARRAY_U8_H

#include <stddef.h>
#include <stdint.h>
#include "tracked_alloc.h"
/* Définition du type */
typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} DynArrayU8;

/* Fonctions principales */
int da_init(DynArrayU8 *da, size_t initial_capacity);
void da_free(DynArrayU8 *da);
int da_push_back(DynArrayU8 *da, uint8_t value);
size_t da_size(const DynArrayU8 *da);

#endif /* DYNARRAY_U8_H */
