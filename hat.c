#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tracked_alloc.h"

typedef struct {
    size_t n;        // nombre d'éléments
    size_t B;        // taille d'un bloc
    uint8_t **dir;   // tableau de B pointeurs vers blocs de B uint8_t, B racine de N
} HAT_U8;

static size_t next_pow2(size_t x) {
    size_t k = 0;
    while (x > ((size_t)1 << k)) {
        k += 1;
    }
    return ((size_t)1 << k);
}

int hat_init(HAT_U8 *h, size_t initial_B) {
  if (!h) return -1;   
  size_t B = next_pow2(initial_B);
  h->n = 0;
  h->B = B;
  h->dir = tracked_calloc(B, sizeof(uint8_t *)); 
  if (!h->dir) return -1;
  return 0; //
}

void hat_free(HAT_U8 *h) {
    if (!h) return;
    for (size_t i = 0; i < h->B; ++i) {
      tracked_free(h->dir[i], h->B);
    }
    tracked_free(h->dir, h->n * sizeof(size_t));    
}

static int hat_rebuild(HAT_U8 *h, size_t newB) {
    if (!h) return -1;
    uint8_t **new_dir = tracked_calloc(newB, sizeof(uint8_t *));
    if (!new_dir) return -1;

    size_t oldB = h->B;
    size_t oldn = h->n; 
    uint8_t **old_dir = h->dir;
    size_t n = h->n;

    for (size_t i = 0; i < n; ++i) {
        size_t old_blk = i / oldB;
        size_t old_off = i & (oldB - 1);
        size_t new_blk = i / newB;
        size_t new_off = i & (newB - 1);

        if (!new_dir[new_blk]) {
            new_dir[new_blk] = tracked_calloc(newB, sizeof(uint8_t));
            if (!new_dir[new_blk]) return -1;
        }

        new_dir[new_blk][new_off] = old_dir[old_blk][old_off];
    }

    // libère les anciens blocs
    for (size_t i = 0; i < oldB; ++i)
      tracked_free(old_dir[i], oldB);
    tracked_free(old_dir, oldn * sizeof(size_t));

    h->dir = new_dir;
    h->B = newB;
    return 0;
}

uint8_t hat_get(HAT_U8 *h, size_t index) {
    if (!h || index >= h->n) return 0;
    size_t b = h->B;
    size_t blk = index / b;
    size_t off = index & (b - 1);
    return h->dir[blk][off];
}

void hat_set(HAT_U8 *h, size_t index, uint8_t value) {
    if (!h || index >= h->n) return;
    size_t b = h->B;
    size_t blk = index / b;
    size_t off = index & (b - 1);
    h->dir[blk][off] = value;
}

int hat_push(HAT_U8 *h, uint8_t value) {
    if (!h) return -1;

    size_t capacity = h->B * h->B;
    if (h->n == capacity) {
        if (hat_rebuild(h, h->B * 2) != 0) return -1;
    }

    size_t idx = h->n;
    size_t b = h->B;
    size_t blk = idx / b;
    size_t off = idx & (b - 1);

    if (!h->dir[blk]) {
        h->dir[blk] = tracked_calloc(b, sizeof(uint8_t));
        if (!h->dir[blk]) return -1;
    }

    h->dir[blk][off] = value;
    h->n++;
    return 0;
}

int hat_pop(HAT_U8 *h, uint8_t *out) {
    if (!h || h->n == 0) return -1;
    h->n--;
    size_t idx = h->n;
    size_t b = h->B;
    size_t blk = idx / b;
    size_t off = idx & (b - 1);
    if (out) *out = h->dir[blk][off];
    if (h->B > 1 && h->n <= (h->B * h->B) / 16) {
        size_t newB = h->B / 2;
        if (newB < 1) newB = 1;
        hat_rebuild(h, newB);
    }
    return 0;
}
