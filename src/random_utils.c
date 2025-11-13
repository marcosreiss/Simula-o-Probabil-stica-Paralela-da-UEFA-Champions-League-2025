void init_random_seed(int seed_extra) {
    srand(time(NULL) + seed_extra);
}

double rand_uniform() {
    return (double) rand() / (double) RAND_MAX;
}

int rand_int(int max) {
    return rand() % max;
}

// implementação de Poisson (clássica, por soma de expoenciais)
int poisson(double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    do {
        k++;
        p *= rand_uniform();
    } while (p > L);
    return k - 1;
}
