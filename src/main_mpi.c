#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "teams.h"
#include "random_utils.h"
#include "sim.h"
#include "stats.h"

int main(int argc, char *argv[])
{
    int rank, size;
    double inicio, fim, tempo_local, tempo_max;

    // Inicializa MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long num_simulacoes_total = 10000000;

    if (argc > 1)
    {
        num_simulacoes_total = atoll(argv[1]);
    }

    // Apenas o processo 0 imprime o cabeçalho
    if (rank == 0)
    {
        printf("=== SIMULADOR MONTE CARLO - UEFA CHAMPIONS LEAGUE 2025 ===\n");
        printf("Modo: MPI\n");
        printf("Número de simulações: %lld\n", num_simulacoes_total);
        printf("Processos MPI: %d\n\n", size);
    }

    // Todos os processos carregam os times
    Time times[NUM_TIMES];
    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);

    if (rank == 0)
    {
        printf("Times carregados e ordenados por força:\n");
        for (int i = 0; i < NUM_TIMES; i++)
        {
            printf("%2d. %-20s (Força: %.2f)\n", i + 1, times[i].nome, times[i].forca);
        }
        printf("\n");
    }

    // Calcula simulações por processo
    long long sims_por_processo = num_simulacoes_total / size;
    long long sims_restantes = num_simulacoes_total % size;

    // Processo 0 pega as simulações extras (se houver resto)
    long long num_sims_local = sims_por_processo;
    if (rank == 0)
    {
        num_sims_local += sims_restantes;
    }

    // Estatísticas locais de cada processo
    Estatisticas estat_local;
    zerar_estatisticas(&estat_local);

    // Seed única por processo
    init_random_seed(rank * 1000 + (unsigned int)time(NULL));

    // Barreira para sincronizar início
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Iniciando simulações...\n");
    }

    // Marca tempo de início
    inicio = MPI_Wtime();

    // Cada processo executa suas simulações
    for (long long sim = 0; sim < num_sims_local; sim++)
    {
        simular_campeonato(times, &estat_local);
    }

    // Marca tempo de fim local
    tempo_local = MPI_Wtime() - inicio;

    // Encontra o maior tempo entre todos os processos (gargalo)
    MPI_Reduce(&tempo_local, &tempo_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Estatísticas globais (apenas no processo 0)
    Estatisticas estat_global;
    zerar_estatisticas(&estat_global);

    // Redução de todas as estatísticas para o processo 0
    MPI_Reduce(estat_local.passou_grupos, estat_global.passou_grupos,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(estat_local.quartas, estat_global.quartas,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(estat_local.semis, estat_global.semis,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(estat_local.finais, estat_global.finais,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(estat_local.titulos, estat_global.titulos,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Processo 0 imprime resultados
    if (rank == 0)
    {
        printf("\nSimulações concluídas!\n");
        printf("Tempo de execução (wall-clock): %.4f segundos\n", tempo_max);
        printf("Simulações por segundo: %.0f\n", num_simulacoes_total / tempo_max);

        // Comparações com versões anteriores
        double tempo_serial_ref = 111.93;
        double tempo_omp_ref = 14.00;

        double speedup_vs_serial = tempo_serial_ref / tempo_max;
        double speedup_vs_omp = tempo_omp_ref / tempo_max;

        printf("\n--- Análise de Performance ---\n");
        printf("Speedup vs Serial: %.2fx (%.2fs → %.2fs)\n", 
               speedup_vs_serial, tempo_serial_ref, tempo_max);
        printf("Speedup vs OpenMP: %.2fx (%.2fs → %.2fs)\n", 
               speedup_vs_omp, tempo_omp_ref, tempo_max);
        printf("Eficiência paralela MPI: %.1f%% (speedup %.2fx / %d processos)\n",
               (speedup_vs_serial / size) * 100.0, speedup_vs_serial, size);
        printf("\n");

        imprimir_estatisticas(&estat_global, times, num_simulacoes_total);
    }

    MPI_Finalize();
    return 0;
}