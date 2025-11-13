#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "teams.h"
#include "random_utils.h"
#include "sim.h"
#include "stats.h"

int main(int argc, char *argv[])
{
    // Configuração do número de simulações
    long long num_simulacoes = 10000;

    if (argc > 1)
    {
        num_simulacoes = atoll(argv[1]);
    }

    printf("=== SIMULADOR MONTE CARLO - UEFA CHAMPIONS LEAGUE 2025 ===\n");
    printf("Modo: SERIAL\n");
    printf("Número de simulações: %lld\n\n", num_simulacoes);

    // Inicializa gerador de números aleatórios
    init_random_seed(0);

    // Carrega os 18 times e calcula suas forças
    Time times[NUM_TIMES];
    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);

    printf("Times carregados e ordenados por força:\n");
    for (int i = 0; i < NUM_TIMES; i++)
    {
        printf("%2d. %-20s (Força: %.2f)\n", i + 1, times[i].nome, times[i].forca);
    }
    printf("\n");

    // Inicializa estrutura de estatísticas global
    Estatisticas estat_global;
    zerar_estatisticas(&estat_global);

    // Inicia contagem de tempo
    clock_t inicio = clock();

    // Loop principal de Monte Carlo
    printf("Iniciando simulações...\n");
    for (long long sim = 0; sim < num_simulacoes; sim++)
    {
        // Estatísticas locais para esta simulação
        Estatisticas estat_local;
        zerar_estatisticas(&estat_local);

        // Simula uma edição completa da Champions
        simular_campeonato(times, &estat_local);

        // Acumula resultados na estatística global
        acumular_estatisticas(&estat_global, &estat_local);

        // Progresso a cada 10%
        if ((sim + 1) % (num_simulacoes / 10) == 0)
        {
            printf("Progresso: %lld/%lld (%.0f%%)\n",
                   sim + 1, num_simulacoes,
                   ((double)(sim + 1) / num_simulacoes) * 100.0);
        }
    }

    // Finaliza contagem de tempo
    clock_t fim = clock();
    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nSimulações concluídas!\n");
    printf("Tempo total: %.2f segundos\n", tempo_total);
    printf("Simulações por segundo: %.0f\n\n", num_simulacoes / tempo_total);

    // Imprime resultados
    imprimir_estatisticas(&estat_global, times, num_simulacoes);

    return 0;
}
