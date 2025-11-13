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
void registrar_resultado_torneio(Estatisticas *e, Time *classificados_quartas[],
                                 Time *classificados_semis[],
                                 Time *finalistas[],
                                 Time *campeao);

#endif
