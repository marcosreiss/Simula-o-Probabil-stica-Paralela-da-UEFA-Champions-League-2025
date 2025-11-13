#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "teams.h"

/*
 * Função auxiliar para comparar times por força (qsort)
 */
static int comparar_por_forca(const void *a, const void *b) {
    Time *t1 = (Time *)a;
    Time *t2 = (Time *)b;
    
    // Ordenação decrescente (mais forte primeiro)
    if (t2->forca > t1->forca) return 1;
    if (t2->forca < t1->forca) return -1;
    return 0;
}

/*
 * Carrega os 18 times com seus atributos base.
 * Estes valores vêm do modelo probabilístico definido anteriormente.
 */
void carregar_times(Time times[]) {
    Time temp[NUM_TIMES] = {
        {"Manchester City", 95, 90, 92, 85, 0.0, 0},
        {"Real Madrid",     93, 88, 90, 100, 0.0, 1},
        {"Bayern Munich",   92, 88, 88, 95, 0.0, 2},
        {"PSG",             91, 85, 87, 80, 0.0, 3},
        {"Liverpool",       90, 87, 86, 90, 0.0, 4},
        {"Barcelona",       89, 84, 85, 95, 0.0, 5},

        {"Arsenal",         88, 84, 87, 75, 0.0, 6},
        {"Inter Milan",     86, 88, 85, 85, 0.0, 7},
        {"Atletico Madrid", 83, 89, 83, 85, 0.0, 8},
        {"Juventus",        84, 86, 80, 90, 0.0, 9},
        {"AC Milan",        84, 82, 82, 88, 0.0, 10},
        {"Borussia Dortmund", 85, 80, 84, 75, 0.0, 11},

        {"RB Leipzig",      82, 80, 83, 65, 0.0, 12},
        {"Napoli",          84, 78, 82, 70, 0.0, 13},
        {"Porto",           80, 80, 81, 80, 0.0, 14},
        {"Benfica",         81, 79, 80, 78, 0.0, 15},
        {"Newcastle",       82, 78, 79, 60, 0.0, 16},
        {"Bayer Leverkusen",83, 79, 86, 65, 0.0, 17}
    };

    for (int i = 0; i < NUM_TIMES; i++) {
        times[i] = temp[i];
    }
}

/*
 * Calcula a força geral de cada time.
 * Fórmula ponderada:
 *   35% ataque, 35% defesa, 20% forma, 10% experiência
 * 
 * Depois de calcular, ordena os times por força (decrescente)
 */
void calcular_forcas(Time times[], int n) {
    for (int i = 0; i < n; i++) {
        times[i].forca =
            0.35 * times[i].atk +
            0.35 * times[i].def +
            0.20 * times[i].form +
            0.10 * times[i].exp;
    }
    
    // Ordena os times por força (decrescente)
    qsort(times, n, sizeof(Time), comparar_por_forca);
    
    // Atualiza os IDs após ordenação
    for (int i = 0; i < n; i++) {
        times[i].id = i;
    }
}

/*
 * Divide os times em 3 potes usando ponteiros:
 * Pote 1 = times mais fortes (índices 0-5)
 * Pote 2 = forças intermediárias (índices 6-11)
 * Pote 3 = times mais fracos (índices 12-17)
 *
 * IMPORTANTE: os times já devem estar ordenados por força
 * (via calcular_forcas) antes de chamar esta função.
 */
void montar_potes(Time times[],
                  Time *pote1[TIMES_POR_POTE],
                  Time *pote2[TIMES_POR_POTE],
                  Time *pote3[TIMES_POR_POTE]) {
    
    // Distribui ponteiros para os times nos potes
    for (int i = 0; i < TIMES_POR_POTE; i++) {
        pote1[i] = &times[i];           // 6 mais fortes
        pote2[i] = &times[i + 6];       // 6 do meio
        pote3[i] = &times[i + 12];      // 6 mais fracos
    }
}
