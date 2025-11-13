#ifndef STATS_H
#define STATS_H

#include "teams.h"

typedef struct {
    long long passou_grupos[NUM_TIMES];
    long long quartas[NUM_TIMES];
    long long semis[NUM_TIMES];
    long long finais[NUM_TIMES];
    long long titulos[NUM_TIMES];
} Estatisticas;

void zerar_estatisticas(Estatisticas *e);
void acumular_estatisticas(Estatisticas *global, Estatisticas *local);
void registrar_campeao(Estatisticas *e, Time *campeao);
void registrar_classificados(Estatisticas *e, Time *classificados[], int n_classificados);
void imprimir_estatisticas(Estatisticas *e, Time times[], long long total_simulacoes);

#endif
