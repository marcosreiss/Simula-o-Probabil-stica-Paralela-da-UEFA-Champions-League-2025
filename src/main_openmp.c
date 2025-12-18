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

    // Detecta e exibe número de threads
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

// Região paralela OpenMP
#pragma omp parallel
    {
        // Cada thread tem sua própria estatística local
        Estatisticas estat_thread;
        zerar_estatisticas(&estat_thread);

        // Inicializa seed única por thread
        int thread_id = omp_get_thread_num();
        init_random_seed(thread_id * 1000 + time(NULL));

// Distribui o loop entre threads
#pragma omp for
        for (long long sim = 0; sim < num_simulacoes; sim++)
        {
            Estatisticas estat_local;
            zerar_estatisticas(&estat_local);

            simular_campeonato(times, &estat_local);
            acumular_estatisticas(&estat_thread, &estat_local);

            // Progresso (apenas thread 0, aproximado)
            if (thread_id == 0 && (sim + 1) % (num_simulacoes / 10) == 0)
            {
                printf("Progresso: ~%lld/%lld (~%.0f%%)\n",
                       sim + 1, num_simulacoes,
                       ((double)(sim + 1) / num_simulacoes) * 100.0);
            }
        }

// Merge thread-safe das estatísticas
#pragma omp critical
        {
            acumular_estatisticas(&estat_global, &estat_thread);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    // Calcula tempo decorrido (wall-clock time)
    double tempo_total = (fim.tv_sec - inicio.tv_sec) +
                         (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("\nSimulações concluídas!\n");
    printf("Tempo de execução (wall-clock): %.4f segundos\n", tempo_total);
    printf("Simulações por segundo: %.0f\n", num_simulacoes / tempo_total);
    printf("Speedup teórico máximo: %dx (baseado em %d threads)\n\n", num_threads, num_threads);

    imprimir_estatisticas(&estat_global, times, num_simulacoes);

    return 0;
}
