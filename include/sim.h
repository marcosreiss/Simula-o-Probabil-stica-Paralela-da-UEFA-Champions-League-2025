#ifndef SIM_H
#define SIM_H

#include "teams.h"
#include "stats.h"

typedef struct {
    int gols_a;
    int gols_b;
} ResultadoJogo;

void embaralhar_times(Time *vet, int n);
void sortear_grupos(Time pote1[], Time pote2[], Time pote3[], Grupo grupos[NUM_GRUPOS]);
ResultadoJogo simular_jogo(Time *a, Time *b);
void simular_fase_grupos(Grupo grupos[NUM_GRUPOS], Time *classificados[8]);
void simular_mata_mata(Time *classificados[8], Time **quartas_out[8],
                       Time **semis_out[4], Time **final_out[2], Time **campeao_out);

void simular_campeonato(Time times[], Estatisticas *estat_local);

#endif
