#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "redimensionnable_naif.h"
#include "hat.h"
#include "tracked_alloc.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nombre_de_tests>\n", argv[0]);
        return 1;
    }

    char *endptr = NULL;
    long m = strtol(argv[1], &endptr, 10);

    FILE *fout = fopen("resultats.csv", "w");
    if (!fout) {
        perror("Impossible d'ouvrir resultats.csv");
        return 1;
    }

    fprintf(fout, "n,impl,temps_sec,alloc_octets\n");

    for (long n = 0; n < m; n++) {
        /* Init de l'implémentation naïve  */
        DynArrayU8 arr;
        if (da_init(&arr, 1) != 0) {
            fprintf(stderr, "Erreur: allocation échouée pour l'implé naïve\n");
            return 1;
        }

        /* Test implémentation naïve */
        clock_t naive_s = clock();
        for (long i = 0; i < n; ++i) {
            if (da_push_back(&arr, 2) != 0) {
                fprintf(stderr, "Erreur: push échoué à l'indice %zu\n", i);
                da_free(&arr);
                return 1;
            }
        }
        size_t total_alloc_naif = get_total_allocated();

        uint8_t k_naif;
        for (long i = 0; i < n; ++i) {
            if (da_pop_back(&arr, &k_naif) != 0) {
                fprintf(stderr, "Erreur: pop échoué à l'indice %zu\n", i);
                da_free(&arr);
                return 1;
            }
        }

        clock_t naive_e = clock();
        double naive_u = ((double)(naive_e - naive_s)) / CLOCKS_PER_SEC;
        fprintf(fout, "%ld,naif,%f,%zu\n", n, naive_u, total_alloc_naif);
        da_free(&arr);

        /* Test implémentation HAT */
        HAT_U8 hat;
        if (hat_init(&hat, 1) != 0) {
            fprintf(stderr, "Erreur: allocation échouée pour HAT\n");
            return 1;
        }

        clock_t hat_s = clock();
        for (long i = 0; i < n; ++i) {
            if (hat_push(&hat, 2) != 0) {
                fprintf(stderr, "Erreur: push HAT échoué à l'indice %zu\n", i);
                hat_free(&hat);
                return 1;
            }
        }
        size_t total_alloc_hat = get_total_allocated();

        uint8_t k_hat;
        for (long i = 0; i < n; ++i) {
            if (hat_pop(&hat, &k_hat) != 0) {
                fprintf(stderr, "Erreur: pop HAT échoué à l'indice %zu\n", i);
                hat_free(&hat);
                return 1;
            }
        }

        clock_t hat_e = clock();
        double hat_u = ((double)(hat_e - hat_s)) / CLOCKS_PER_SEC;
        fprintf(fout, "%ld,HAT,%f,%zu\n", n, hat_u, total_alloc_hat);
        hat_free(&hat);
    }

    fclose(fout);
    return 0;
}
