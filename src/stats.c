#include <stdio.h>
#include <string.h>
#include "stats.h"

void zerar_estatisticas(Estatisticas *e)
{
    memset(e, 0, sizeof(Estatisticas));
}

void acumular_estatisticas(Estatisticas *global, Estatisticas *local)
{
    for (int i = 0; i < NUM_TIMES; i++)
    {
        global->passou_grupos[i] += local->passou_grupos[i];
        global->quartas[i] += local->quartas[i];
        global->semis[i] += local->semis[i];
        global->finais[i] += local->finais[i];
        global->titulos[i] += local->titulos[i];
    }
}

void registrar_campeao(Estatisticas *e, Time *campeao)
{
    if (campeao != NULL)
    {
        e->titulos[campeao->id]++;
        e->finais[campeao->id]++;
    }
}

void registrar_classificados(Estatisticas *e, Time *classificados[], int n_classificados)
{
    for (int i = 0; i < n_classificados; i++)
    {
        if (classificados[i] != NULL)
        {
            e->quartas[classificados[i]->id]++;
            e->passou_grupos[classificados[i]->id]++;
        }
    }
}

void imprimir_estatisticas(Estatisticas *e, Time times[], long long total_simulacoes)
{
    printf("\n=== RESULTADOS APÓS %lld SIMULAÇÕES ===\n\n", total_simulacoes);
    printf("%-20s %10s %10s %10s %10s %10s\n",
           "TIME", "GRUPOS%", "QUARTAS%", "SEMIS%", "FINAL%", "TÍTULO%");
    printf("------------------------------------------------------------------------------------\n");

    for (int i = 0; i < NUM_TIMES; i++)
    {
        double p_grupos = (double)e->passou_grupos[i] / total_simulacoes * 100.0;
        double p_quartas = (double)e->quartas[i] / total_simulacoes * 100.0;
        double p_semis = (double)e->semis[i] / total_simulacoes * 100.0;
        double p_finais = (double)e->finais[i] / total_simulacoes * 100.0;
        double p_titulos = (double)e->titulos[i] / total_simulacoes * 100.0;

        printf("%-20s %9.2f%% %9.2f%% %9.2f%% %9.2f%% %9.2f%%\n",
               times[i].nome, p_grupos, p_quartas, p_semis, p_finais, p_titulos);
    }
}
