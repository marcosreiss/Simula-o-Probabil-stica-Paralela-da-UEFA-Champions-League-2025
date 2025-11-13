#include <stdio.h>
#include "teams.h"
#include "sim.h"
#include "stats.h"
#include "random_utils.h"

int main(int argc, char *argv[]) {
    int N = 1000; // número de simulações (pode ler da linha de comando)
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    Time times[NUM_TIMES];
    Estatisticas estat_global, estat_local;

    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);
    zerar_estatisticas(&estat_global);

    init_random_seed(0);

    for (int i = 0; i < N; i++) {
        zerar_estatisticas(&estat_local);
        simular_campeonato(times, &estat_local);
        acumular_estatisticas(&estat_global, &estat_local);
    }

    // imprimir probabilidades
    for (int i = 0; i < NUM_TIMES; i++) {
        double p_titulo = (double)estat_global.titulos[i] / (double)N * 100.0;
        printf("%s: %.2f%% títulos\n", times[i].nome, p_titulo);
    }

    return 0;
}
