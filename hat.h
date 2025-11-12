#ifndef DYNARRAY_HAT_U8
#define DYNARRAY_HAT_U8

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "tracked_alloc.h"

typedef struct {
  size_t n;
  size_t B;
  uint8_t **dir;
} HAT_U8;

int hat_init(HAT_U8 *hat, size_t initial_B);
void hat_free(HAT_U8 *hat);
int hat_push(HAT_U8 *hat, uint8_t value);
int hat_pop(HAT_U8 *hat, uint8_t *out); 

#endif /* HAT_U8 */
