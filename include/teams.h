#ifndef TEAMS_H
#define TEAMS_H

#define NUM_TIMES 18
#define NUM_GRUPOS 3
#define TIMES_POR_GRUPO 6

typedef struct {
    char nome[50];
    double atk;
    double def;
    double form;
    double exp;
    double forca;
    int id; // opcional
} Time;

typedef struct {
    int id_time;
    int pontos;
    int gols_pro;
    int gols_contra;
    int saldo;
} EntradaTabela;

typedef struct {
    Time *times[TIMES_POR_GRUPO];
    EntradaTabela tabela[TIMES_POR_GRUPO];
} Grupo;

void carregar_times(Time times[]);
void calcular_forcas(Time times[], int n);
void montar_potes(Time times[], Time pote1[], Time pote2[], Time pote3[]);

#endif
