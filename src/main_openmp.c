#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "teams.h"
#include "random_utils.h"
#include "sim.h"
#include "stats.h"

int main(int argc, char *argv[])
{
    long long num_simulacoes = 10000;

    if (argc > 1)
    {
        num_simulacoes = atoll(argv[1]);
    }

    printf("=== SIMULADOR MONTE CARLO - UEFA CHAMPIONS LEAGUE 2025 ===\n");
    printf("Modo: OPENMP\n");
    printf("Número de simulações: %lld\n", num_simulacoes);

    int num_threads;
#pragma omp parallel
    {
#pragma omp single
        num_threads = omp_get_num_threads();
    }
    printf("Threads OpenMP: %d\n\n", num_threads);

    Time times[NUM_TIMES];
    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);

    printf("Times carregados e ordenados por força:\n");
    for (int i = 0; i < NUM_TIMES; i++)
    {
        printf("%2d. %-20s (Força: %.2f)\n", i + 1, times[i].nome, times[i].forca);
    }
    printf("\n");

    Estatisticas estat_global;
    zerar_estatisticas(&estat_global);

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    printf("Iniciando simulações...\n");

#pragma omp parallel
    {
        Estatisticas estat_thread;
        zerar_estatisticas(&estat_thread);

        int thread_id = omp_get_thread_num();
        init_random_seed(thread_id * 1000 + (unsigned int)time(NULL));

#pragma omp for schedule(dynamic, 10000)
        for (long long sim = 0; sim < num_simulacoes; sim++)
        {
            simular_campeonato(times, &estat_thread);
        }

#pragma omp critical
        {
            acumular_estatisticas(&estat_global, &estat_thread);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo_total = (fim.tv_sec - inicio.tv_sec) +
                         (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("\nSimulações concluídas!\n");
    printf("Tempo de execução (wall-clock): %.4f segundos\n", tempo_total);
    printf("Simulações por segundo: %.0f\n", num_simulacoes / tempo_total);

    double tempo_serial_ref = 111.93;
    double speedup_real = tempo_serial_ref / tempo_total;
    printf("Speedup real: %.2fx (vs %.2fs serial)\n", speedup_real, tempo_serial_ref);
    printf("Eficiência paralela: %.1f%%\n\n", (speedup_real / num_threads) * 100.0);

    imprimir_estatisticas(&estat_global, times, num_simulacoes);

    return 0;
}
