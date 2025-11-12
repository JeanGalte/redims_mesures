#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "redimensionnable_naif.h"
#include "tracked_alloc.h"

int main(int argc, char *argv[]) {
  
  char *endptr = NULL;
  long n = strtol(argv[1], &endptr, 10); 

  printf("Tests lancés pour n=%ld\n", n); 
 
  /* Init de l'implémentation naïve  */
  DynArrayU8 arr;
  if (da_init(&arr, 4) != 0) {
    fprintf(stderr, "Erreur: allocation échouée.\n");
    return 1;
  }
 
  /* Lancement du test pour l'implé naïve */  
  
  clock_t naive_s, naive_e;
  double naive_u;
  naive_s = clock();
  /* Insérer n éléments */
  for (long i = 0; i < n; ++i) {
    if (da_push_back(&arr, 2) != 0) {
      fprintf(stderr, "Erreur: échec lors du push à l’indice %zu.\n", i);
      da_free(&arr);
      return 1;
    }
  }
  /* Retirer n éléments  */
  uint8_t k = 0; 
  for (long i = 0; i < n; ++i) {
    if (da_push_back(&arr, k) != 0) {
      fprintf(stderr, "Erreur: échec lors du pop à numéro %zu.\n", i);
      da_free(&arr);
      return 1;
    }
  }
  
  naive_e = clock();
  naive_u = ((double)(naive_e - naive_s)) / CLOCKS_PER_SEC;
  printf("[*] Implémentation naïve : %f secondes, %zu octets alloués", naive_u, get_total_allocated()); 
  da_free(&arr);

  /* HAT_U8 *hat = hat_create(4); */
  /* if (!hat) { */
  /*   fprintf(sterr, "Erreur : échec lors de la création du hat"); */
  /*   return 1;  */
  /* } */

  
  
  
  return 0;
}
