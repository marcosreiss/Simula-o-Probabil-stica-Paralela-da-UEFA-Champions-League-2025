#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "random_utils.h"

/*
 * Inicializa o gerador de números aleatórios.
 * seed_extra permite variação entre threads/processos.
 */
void init_random_seed(int seed_extra) {
    srand(time(NULL) + seed_extra);
}

/*
 * Retorna número aleatório uniforme entre 0.0 e 1.0
 */
double rand_uniform() {
    return (double)rand() / (double)RAND_MAX;
}

/*
 * Retorna inteiro aleatório entre 0 e max-1.
 * Protegido contra max <= 0.
 */
int rand_int(int max) {
    if (max <= 0) return 0;
    return rand() % max;
}

/*
 * Gera número aleatório seguindo distribuição de Poisson.
 * Implementação clássica por soma de exponenciais.
 * Usado para simular gols nas partidas.
 */
int poisson(double lambda) {
    if (lambda <= 0.0) return 0;
    
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    
    do {
        k++;
        p *= rand_uniform();
    } while (p > L);
    
    return k - 1;
}
