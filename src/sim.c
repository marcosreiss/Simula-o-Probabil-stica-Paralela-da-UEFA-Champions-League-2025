#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.h"
#include "teams.h"
#include "stats.h"
#include "random_utils.h"

/* ========== FUNÇÕES AUXILIARES ========== */

void embaralhar_times(Time **vet, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand_int(i + 1);
        Time *temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}

/* ========== SORTEIO DOS GRUPOS ========== */

void sortear_grupos(Time *pote1[TIMES_POR_POTE],
                    Time *pote2[TIMES_POR_POTE],
                    Time *pote3[TIMES_POR_POTE],
                    Grupo grupos[NUM_GRUPOS])
{

    // Embaralha cada pote
    embaralhar_times(pote1, TIMES_POR_POTE);
    embaralhar_times(pote2, TIMES_POR_POTE);
    embaralhar_times(pote3, TIMES_POR_POTE);

    // Distribui 2 times de cada pote por grupo (6 times por grupo)
    for (int g = 0; g < NUM_GRUPOS; g++)
    {
        grupos[g].id_grupo = g;

        // 2 do pote 1
        grupos[g].times[0] = pote1[g * 2];
        grupos[g].times[1] = pote1[g * 2 + 1];

        // 2 do pote 2
        grupos[g].times[2] = pote2[g * 2];
        grupos[g].times[3] = pote2[g * 2 + 1];

        // 2 do pote 3
        grupos[g].times[4] = pote3[g * 2];
        grupos[g].times[5] = pote3[g * 2 + 1];

        // Inicializa tabela
        for (int i = 0; i < TIMES_POR_GRUPO; i++)
        {
            grupos[g].tabela[i].id_time = i;
            grupos[g].tabela[i].pontos = 0;
            grupos[g].tabela[i].vitorias = 0;
            grupos[g].tabela[i].empates = 0;
            grupos[g].tabela[i].derrotas = 0;
            grupos[g].tabela[i].gols_pro = 0;
            grupos[g].tabela[i].gols_contra = 0;
            grupos[g].tabela[i].saldo = 0;
            grupos[g].tabela[i].jogos = 0;
        }
    }
}

/* ========== SIMULAÇÃO DE JOGO ========== */

ResultadoJogo simular_jogo(Time *a, Time *b)
{
    ResultadoJogo resultado;

    // Calcula lambda para cada time (média de gols esperada)
    // Fórmula: lambda = (ATK_atacante + (100 - DEF_defensor)) / 100 * fator_forma
    double lambda_a = ((a->atk + (100.0 - b->def)) / 100.0) * (a->form / 100.0) * 1.5;
    double lambda_b = ((b->atk + (100.0 - a->def)) / 100.0) * (b->form / 100.0) * 1.5;

    // Gera gols usando distribuição de Poisson
    resultado.gols_a = poisson(lambda_a);
    resultado.gols_b = poisson(lambda_b);

    return resultado;
}

/* ========== FASE DE GRUPOS ========== */

static int comparar_tabela(const void *a, const void *b)
{
    EntradaTabela *t1 = (EntradaTabela *)a;
    EntradaTabela *t2 = (EntradaTabela *)b;

    // 1. Pontos (decrescente)
    if (t2->pontos != t1->pontos)
        return t2->pontos - t1->pontos;

    // 2. Saldo de gols (decrescente)
    if (t2->saldo != t1->saldo)
        return t2->saldo - t1->saldo;

    // 3. Gols pró (decrescente)
    return t2->gols_pro - t1->gols_pro;
}

void simular_fase_grupos(Grupo grupos[NUM_GRUPOS], Time *classificados[8])
{
    for (int g = 0; g < NUM_GRUPOS; g++)
    {
        // Simula todos contra todos (ida e volta)
        for (int i = 0; i < TIMES_POR_GRUPO; i++)
        {
            for (int j = i + 1; j < TIMES_POR_GRUPO; j++)
            {
                // Jogo de ida
                ResultadoJogo r1 = simular_jogo(grupos[g].times[i], grupos[g].times[j]);

                // Atualiza tabela (ida)
                grupos[g].tabela[i].gols_pro += r1.gols_a;
                grupos[g].tabela[i].gols_contra += r1.gols_b;
                grupos[g].tabela[j].gols_pro += r1.gols_b;
                grupos[g].tabela[j].gols_contra += r1.gols_a;
                grupos[g].tabela[i].jogos++;
                grupos[g].tabela[j].jogos++;

                if (r1.gols_a > r1.gols_b)
                {
                    grupos[g].tabela[i].pontos += 3;
                    grupos[g].tabela[i].vitorias++;
                    grupos[g].tabela[j].derrotas++;
                }
                else if (r1.gols_a < r1.gols_b)
                {
                    grupos[g].tabela[j].pontos += 3;
                    grupos[g].tabela[j].vitorias++;
                    grupos[g].tabela[i].derrotas++;
                }
                else
                {
                    grupos[g].tabela[i].pontos += 1;
                    grupos[g].tabela[j].pontos += 1;
                    grupos[g].tabela[i].empates++;
                    grupos[g].tabela[j].empates++;
                }

                // Jogo de volta
                ResultadoJogo r2 = simular_jogo(grupos[g].times[j], grupos[g].times[i]);

                grupos[g].tabela[j].gols_pro += r2.gols_a;
                grupos[g].tabela[j].gols_contra += r2.gols_b;
                grupos[g].tabela[i].gols_pro += r2.gols_b;
                grupos[g].tabela[i].gols_contra += r2.gols_a;
                grupos[g].tabela[i].jogos++;
                grupos[g].tabela[j].jogos++;

                if (r2.gols_a > r2.gols_b)
                {
                    grupos[g].tabela[j].pontos += 3;
                    grupos[g].tabela[j].vitorias++;
                    grupos[g].tabela[i].derrotas++;
                }
                else if (r2.gols_a < r2.gols_b)
                {
                    grupos[g].tabela[i].pontos += 3;
                    grupos[g].tabela[i].vitorias++;
                    grupos[g].tabela[j].derrotas++;
                }
                else
                {
                    grupos[g].tabela[i].pontos += 1;
                    grupos[g].tabela[j].pontos += 1;
                    grupos[g].tabela[i].empates++;
                    grupos[g].tabela[j].empates++;
                }
            }
        }

        // Atualiza saldos
        for (int i = 0; i < TIMES_POR_GRUPO; i++)
        {
            grupos[g].tabela[i].saldo = grupos[g].tabela[i].gols_pro - grupos[g].tabela[i].gols_contra;
        }

        // Ordena tabela
        qsort(grupos[g].tabela, TIMES_POR_GRUPO, sizeof(EntradaTabela), comparar_tabela);
    }

    // Classifica 2 primeiros de cada grupo (6 times) + 2 melhores terceiros
    int idx = 0;
    for (int g = 0; g < NUM_GRUPOS; g++)
    {
        classificados[idx++] = grupos[g].times[grupos[g].tabela[0].id_time];
        classificados[idx++] = grupos[g].times[grupos[g].tabela[1].id_time];
    }

    // Encontra os 2 melhores terceiros colocados
    EntradaTabela terceiros[NUM_GRUPOS];
    for (int g = 0; g < NUM_GRUPOS; g++)
    {
        terceiros[g] = grupos[g].tabela[2];
    }
    qsort(terceiros, NUM_GRUPOS, sizeof(EntradaTabela), comparar_tabela);

    for (int i = 0; i < 2; i++)
    {
        for (int g = 0; g < NUM_GRUPOS; g++)
        {
            if (grupos[g].tabela[2].id_time == terceiros[i].id_time &&
                grupos[g].tabela[2].pontos == terceiros[i].pontos)
            {
                classificados[idx++] = grupos[g].times[terceiros[i].id_time];
                break;
            }
        }
    }
}

/* ========== MATA-MATA ========== */

Time *simular_partida_mata_mata(Time *a, Time *b)
{
    ResultadoJogo r = simular_jogo(a, b);

    if (r.gols_a > r.gols_b)
        return a;
    if (r.gols_b > r.gols_a)
        return b;

    // Empate: decide por força
    return (a->forca > b->forca) ? a : b;
}

void simular_mata_mata(Time *classificados[8], Time **campeao)
{
    Time *quartas[4];
    Time *semis[2];

    // Quartas de final
    quartas[0] = simular_partida_mata_mata(classificados[0], classificados[7]);
    quartas[1] = simular_partida_mata_mata(classificados[1], classificados[6]);
    quartas[2] = simular_partida_mata_mata(classificados[2], classificados[5]);
    quartas[3] = simular_partida_mata_mata(classificados[3], classificados[4]);

    // Semifinais
    semis[0] = simular_partida_mata_mata(quartas[0], quartas[1]);
    semis[1] = simular_partida_mata_mata(quartas[2], quartas[3]);

    // Final
    *campeao = simular_partida_mata_mata(semis[0], semis[1]);
}

/* ========== ORQUESTRAÇÃO ========== */

void simular_campeonato(Time times[], Estatisticas *estat_local)
{
    Time *pote1[TIMES_POR_POTE];
    Time *pote2[TIMES_POR_POTE];
    Time *pote3[TIMES_POR_POTE];

    Grupo grupos[NUM_GRUPOS];
    Time *classificados[8];
    Time *campeao = NULL;

    montar_potes(times, pote1, pote2, pote3);
    sortear_grupos(pote1, pote2, pote3, grupos);
    simular_fase_grupos(grupos, classificados);
    simular_mata_mata(classificados, &campeao);

    registrar_campeao(estat_local, campeao);
    registrar_classificados(estat_local, classificados, 8);
}
