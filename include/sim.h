#ifndef SIM_H
#define SIM_H

#include "teams.h"
#include "stats.h"

typedef struct
{
    int gols_a;
    int gols_b;
} ResultadoJogo;

/* Funções auxiliares */
void embaralhar_times(Time **vet, int n);

/* Sorteio dos grupos */
void sortear_grupos(Time *pote1[TIMES_POR_POTE],
                    Time *pote2[TIMES_POR_POTE],
                    Time *pote3[TIMES_POR_POTE],
                    Grupo grupos[NUM_GRUPOS]);

/* Simulação de partidas */
ResultadoJogo simular_jogo(Time *a, Time *b);

/* Fase de grupos */
void simular_fase_grupos(Grupo grupos[NUM_GRUPOS], Time *classificados[8]);

/* Mata-mata - CORRIGIDO: adiciona parâmetro Estatisticas */
void simular_mata_mata(Time *classificados[8], Time **campeao, Estatisticas *e);

/* Função principal: simula uma edição completa da Champions */
void simular_campeonato(Time times[], Estatisticas *estat_local);

#endif
