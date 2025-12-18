#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "random_utils.h"

// Define M_PI se não estiver disponível
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * Inicializa o gerador de números aleatórios.
 * seed_extra permite variação entre threads/processos.
 */
void init_random_seed(int seed_extra)
{
    srand(time(NULL) + seed_extra);
}

/*
 * Retorna número aleatório uniforme entre 0.0 e 1.0
 */
double rand_uniform()
{
    return (double)rand() / (double)RAND_MAX;
}

/*
 * Retorna inteiro aleatório entre 0 e max-1.
 * Protegido contra max <= 0.
 */
int rand_int(int max)
{
    if (max <= 0)
        return 0;
    return rand() % max;
}

/*
 * Gera número aleatório seguindo distribuição de Poisson.
 * Usa método de Knuth para lambda pequeno e aproximação Normal para lambda grande.
 * Usado para simular gols nas partidas.
 */
int poisson(double lambda)
{
    if (lambda <= 0.0)
        return 0;

    // Para lambda grande (>10), usa aproximação Normal
    // Evita loop infinito no método de Knuth
    if (lambda > 10.0)
    {
        // Aproximação: Poisson(lambda) ≈ Normal(lambda, sqrt(lambda))
        double u1 = rand_uniform();
        double u2 = rand_uniform();
        double normal = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        int result = (int)(lambda + sqrt(lambda) * normal + 0.5);
        return (result < 0) ? 0 : result;
    }

    // Método de Knuth para lambda pequeno (<=10)
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do
    {
        k++;
        p *= rand_uniform();
    } while (p > L && k < 100); // Adiciona limite de segurança

    return k - 1;
}
