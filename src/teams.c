#include <stdio.h>
#include <string.h>
#include "teams.h"

/*
 * Carrega os 18 times com seus atributos base.
 * Estes valores vêm do modelo probabilístico definido anteriormente.
 */
void carregar_times(Time times[]) {

    Time temp[NUM_TIMES] = {
        {"Manchester City", 95, 90, 92, 85, 0, 0},
        {"Real Madrid",     93, 88, 90, 100, 0, 1},
        {"Bayern Munich",   92, 88, 88, 95, 0, 2},
        {"PSG",             91, 85, 87, 80, 0, 3},
        {"Liverpool",       90, 87, 86, 90, 0, 4},
        {"Barcelona",       89, 84, 85, 95, 0, 5},

        {"Arsenal",         88, 84, 87, 75, 0, 6},
        {"Inter Milan",     86, 88, 85, 85, 0, 7},
        {"Atletico Madrid", 83, 89, 83, 85, 0, 8},
        {"Juventus",        84, 86, 80, 90, 0, 9},
        {"AC Milan",        84, 82, 82, 88, 0, 10},
        {"Borussia Dortmund", 85, 80, 84, 75, 0, 11},

        {"RB Leipzig",      82, 80, 83, 65, 0, 12},
        {"Napoli",          84, 78, 82, 70, 0, 13},
        {"Porto",           80, 80, 81, 80, 0, 14},
        {"Benfica",         81, 79, 80, 78, 0, 15},
        {"Newcastle",       82, 78, 79, 60, 0, 16},
        {"Bayer Leverkusen",83, 79, 86, 65, 0, 17}
    };

    for (int i = 0; i < NUM_TIMES; i++) {
        times[i] = temp[i];
    }
}

/*
 * Calcula a força geral de cada time.
 * Fórmula simples, ponderada:
 *   35% ataque, 35% defesa, 20% forma, 10% experiência
 */
void calcular_forcas(Time times[], int n) {
    for (int i = 0; i < n; i++) {
        times[i].forca =
            0.35 * times[i].atk +
            0.35 * times[i].def +
            0.20 * times[i].form +
            0.10 * times[i].exp;
    }
}

/*
 * Divide os times em 3 potes:
 * Pote 1 = times mais fortes
 * Pote 2 = forças intermediárias
 * Pote 3 = times mais fracos
 *
 * Aqui estamos usando divisão manual:
 *  - Pote 1: os 6 primeiros do vetor (já ordenado na mão)
 *  - Pote 2: os 6 seguintes
 *  - Pote 3: os 6 últimos
 *
 * Simples, eficiente e funciona para o nosso projeto.
 */
void montar_potes(Time times[],
                  Time pote1[6],
                  Time pote2[6],
                  Time pote3[6]) {

    // Os times já estão ordenados por força no vetor "times".
    // Só distribuímos os 6 primeiros, 6 do meio, 6 do fim.

    for (int i = 0; i < 6; i++) {
        pote1[i] = times[i];
        pote2[i] = times[i + 6];
        pote3[i] = times[i + 12];
    }
}
