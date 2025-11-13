#ifndef TEAMS_H
#define TEAMS_H

#define NUM_TIMES 18
#define NUM_GRUPOS 3
#define TIMES_POR_GRUPO 6

typedef struct
{
    char nome[50];
    double atk;   // ataque (0–100)
    double def;   // defesa (0–100)
    double form;  // forma atual (0–100)
    double exp;   // experiência (0–100)
    double forca; // força calculada
    int id;       // identificador único
} Time;

typedef struct
{
    int id_time; // referência ao índice do time no vetor principal
    int pontos;
    int gols_pro;
    int gols_contra;
    int saldo;
} EntradaTabela;

typedef struct
{
    Time *times[TIMES_POR_GRUPO]; // ponteiros para os times do grupo
    EntradaTabela tabela[TIMES_POR_GRUPO];
} Grupo;

/* Funções */
void carregar_times(Time times[]);
void calcular_forcas(Time times[], int n);
void montar_potes(Time times[],
                  Time pote1[6],
                  Time pote2[6],
                  Time pote3[6]);

#endif
