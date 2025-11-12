#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int n;          // nombre d'éléments
    int B;          // taille d'un bloc (plus petite puissance de 2 au dessus de ce qui est donnée pour correspondre au conseil d'implémentation du papier)
    uint8_t **dir;  // tableau de B pointeurs vers blocs de B uint8_t
} HAT_U8;

static int next_pow2(int x) {
  int k = 0;
  while (x > (1 << k)) {
    k += 1; 
  }
  return (1 << k); 
}

int hat_create(HAT_U8 *h, int initial_B) {
  if !h return -1; 
  int B = next_pow2(initial_B);
  h->n = 0;
  h->B = B;
  h->dir = tracked_calloc(B, sizeof(uint8_t *));
  if (!h->dir) return -1;
  return 0
}

void hat_free(HAT_U8 *h) {
    if (!h) return;
    for (int i = 0; i < h->B; ++i)
      tracked_free(h->dir[i], B * sizeof(uint8_t *));
    free(h->dir);
    free(h);
}

static int hat_rebuild(HAT_U8 *h, int newB) {
    if (!h) return -1;
    uint8_t **new_dir = tracked_calloc(newB, sizeof(uint8_t *));
    if (!new_dir) return -1;

    int oldB = h->B;
    uint8_t **old_dir = h->dir;
    int n = h->n;
    for (int i = 0; i < n; ++i) {
        int old_blk = i / oldB;
        int old_off = i & (oldB - 1);
        int new_blk = i / newB;
        int new_off = i & (newB - 1);

        if (!new_dir[new_blk]) {
            new_dir[new_blk] = tracked_calloc(newB, sizeof(uint8_t));
            if (!new_dir[new_blk]) return -1;
        }

        new_dir[new_blk][new_off] = old_dir[old_blk][old_off];
    }

    // libère les anciens blocs
    for (int i = 0; i < oldB; ++i)
        free(old_dir[i]);
    free(old_dir);

    h->dir = new_dir;
    h->B = newB;
    return 0;
}

uint8_t hat_get(HAT_U8 *h, int index) {
    if (!h || index < 0 || index >= h->n) return 0;
    int b = h->B;
    int blk = index / b;
    int off = index & (b - 1);
    return h->dir[blk][off];
}

void hat_set(HAT_U8 *h, int index, uint8_t value) {
    if (!h || index < 0 || index >= h->n) return;
    int b = h->B;
    int blk = index / b;
    int off = index & (b - 1);
    h->dir[blk][off] = value;
}

int hat_push(HAT_U8 *h, uint8_t value) {
    if (!h) return -1;

    int capacity = h->B * h->B;
    if (h->n == capacity) {
        if (hat_rebuild(h, h->B * 2) != 0) return -1;
    }

    int idx = h->n;
    int b = h->B;
    int blk = idx / b;
    int off = idx & (b - 1);

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
    int idx = h->n;
    int b = h->B;
    int blk = idx / b;
    int off = idx & (b - 1);
    if (out) *out = h->dir[blk][off];
    if (h->B > 1 && h->n <= (h->B * h->B) / 16) {
        int newB = h->B / 2;
        if (newB < 1) newB = 1;
        hat_rebuild(h, newB);
    }
    return 0;
}

