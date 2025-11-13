#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

double rand_uniform();         // 0..1
int rand_int(int max);         // 0..max-1
int poisson(double lambda);    // gerar gols
void init_random_seed(int seed_extra);

#endif
