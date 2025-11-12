#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "redimensionnable_naif.h"
#include "hat.h"
#include "tracked_alloc.h"

int main(int argc, char *argv[]) {
  
  char *endptr = NULL;
  long n = strtol(argv[1], &endptr, 10);
  
  printf("Tests lancés pour n=%ld\n", n); 
 
  /* Init de l'implémentation naïve  */
  DynArrayU8 arr;
  if (da_init(&arr, 1) != 0) {
    fprintf(stderr, "Erreur: allocation échouée pour l'implé naïve\n");
    return 1;
  }
 
  /* Lancement du test pour l'implé naïve */  
  
  clock_t naive_s, naive_e;
  double naive_u;
  naive_s = clock();
  /* Insérer n éléments */
  for (long i = 0; i < n ; ++i) {
    if (da_push_back(&arr, 2) != 0) {
      fprintf(stderr, "Erreur: échec lors du push à l’indice %zu pour le HAT\n", i);
      da_free(&arr);
      return 1;
    }
  }
  size_t total_alloc_naif = get_total_allocated();
  
  uint8_t k_naif = 0;
  for (long i = 0; i < n; ++i) {
    if (da_pop_back(&arr, &k_naif) != 0) {
      fprintf(stderr, "Erreur: échec lors du pop à numéro %zu.\n", i);
      da_free(&arr);
      return 1;
    }
  }
  
  naive_e = clock();
  naive_u = ((double)(naive_e - naive_s)) / CLOCKS_PER_SEC;
  printf("[*] Implémentation naïve : %f secondes, %zu octets alloués\n", naive_u, total_alloc_naif); 
  da_free(&arr);

  /* Init de l'implémentation HAT  */
  HAT_U8 hat;
  if (hat_init(&hat, 1) != 0) {
    fprintf(stderr, "Erreur: allocation échouée pour le HAT \n");
    return 1;
  }
 
  /* Lancement du test pour l'implé HAT  */  
  
  clock_t hat_s, hat_e;
  double hat_u;
  hat_s = clock();
  /* Insérer n éléments */
  for (long i = 0; i < n ; ++i) {
    if (hat_push(&hat, 2) != 0) {
      fprintf(stderr, "Erreur: échec lors du push à l’indice %zu pour le HAT\n", i);
      da_free(&arr);
      return 1;
    }
  }
  
  size_t total_alloc_hat = get_total_allocated();
  
  uint8_t k_hat = 0;
  for (long i = 0; i < n; ++i) {
    if (hat_pop(&hat, &k_hat) != 0) {
      fprintf(stderr, "Erreur: échec lors du pop à numéro %zu.\n", i);
      da_free(&arr);
      return 1;
    }
  }
  
  hat_e = clock();
  hat_u = ((double)(hat_e - hat_s)) / CLOCKS_PER_SEC;
  printf("[*] HAT : %f secondes, %zu octets alloués", hat_u, total_alloc_hat); 
  hat_free(&hat); 
  
  
  return 0;
}
