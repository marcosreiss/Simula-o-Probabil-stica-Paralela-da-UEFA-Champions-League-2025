## 1. Estrutura de pastas e arquivos

Algo enxuto e organizado pra TCC / projeto de disciplina:

```
projeto-champions/
  include/
    teams.h
    sim.h
    stats.h
    random_utils.h
  src/
    main.c
    teams.c
    sim.c
    stats.c
    random_utils.c
  Makefile

```

- `teams.*`: definição dos times, atributos, cálculo da força, potes, etc.
- `sim.*`: simula UMA edição da Champions (sorteio + grupos + mata-mata).
- `stats.*`: acumula estatísticas das simulações.
- `random_utils.*`: funções de aleatoriedade (Poisson, uniformes, seed etc.).
- `main.c`: controla o Monte Carlo (loop de N simulações)
    - versão serial
    - versão OpenMP
    - versão MPI

---

## 2. Structs principais

### `teams.h`

```c
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

```

### `stats.h`

```c
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

```

### `random_utils.h`

```c
#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

double rand_uniform();         // 0..1
int rand_int(int max);         // 0..max-1
int poisson(double lambda);    // gerar gols
void init_random_seed(int seed_extra);

#endif

```

### `sim.h`

```c
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

```

---

## 3. Funções por módulo (o que cada arquivo faz)

### `teams.c`

Responsável por:

- preencher o array `times[18]` com aqueles valores que definimos (ATK, DEF, FORM, EXP);
- calcular `forca` de cada time;
- montar os potes 1, 2 e 3.

Funções:

```c
void carregar_times(Time times[]);
void calcular_forcas(Time times[], int n);
void montar_potes(Time times[], Time pote1[], Time pote2[], Time pote3[]);

```

### `random_utils.c`

- inicializar seed (diferente por processo MPI / thread, se quiser)
- gerar números aleatórios
- implementação de Poisson simples

```c
void init_random_seed(int seed_extra) {
    srand(time(NULL) + seed_extra);
}

double rand_uniform() {
    return (double) rand() / (double) RAND_MAX;
}

int rand_int(int max) {
    return rand() % max;
}

// implementação de Poisson (clássica, por soma de expoenciais)
int poisson(double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    do {
        k++;
        p *= rand_uniform();
    } while (p > L);
    return k - 1;
}

```

### `sim.c`

Aqui mora o coração da coisa: **uma edição da Champions**.

Principais blocos:

1. `embaralhar_times` → Fisher–Yates
2. `sortear_grupos` → usa potes e preenche `Grupo grupos[3]`
3. `simular_jogo` → usando o modelo probabilístico (força → média de gols → Poisson)
4. `simular_fase_grupos` → gera tabela, roda todos os jogos ida/volta, ordena, escolhe classificados
5. `simular_mata_mata` → quartas, semis, final
6. `simular_campeonato` → junta tudo acima e, no fim, chama `registrar_resultado_torneio`

Pseudocódigo de `simular_campeonato`:

```c
void simular_campeonato(Time times[], Estatisticas *estat_local) {
    Time pote1[6], pote2[6], pote3[6];
    Grupo grupos[NUM_GRUPOS];
    Time *classificados_quartas[8];
    Time *classificados_semis[4];
    Time *finalistas[2];
    Time *campeao = NULL;

    montar_potes(times, pote1, pote2, pote3);
    sortear_grupos(pote1, pote2, pote3, grupos);
    simular_fase_grupos(grupos, classificados_quartas);
    simular_mata_mata(classificados_quartas,
                      &classificados_quartas, // ou null se não quiser detalhar
                      &classificados_semis,
                      &finalistas,
                      &campeao);

    registrar_resultado_torneio(estat_local,
                                classificados_quartas,
                                classificados_semis,
                                finalistas,
                                campeao);
}

```

### `stats.c`

- inicializar e acumular estatísticas
- somar stats locais de cada simulação/thread/processo em um global

```c
void zerar_estatisticas(Estatisticas *e) {
    memset(e, 0, sizeof(Estatisticas));
}

void acumular_estatisticas(Estatisticas *global, Estatisticas *local) {
    for (int i = 0; i < NUM_TIMES; i++) {
        global->passou_grupos[i] += local->passou_grupos[i];
        global->quartas[i]       += local->quartas[i];
        global->semis[i]         += local->semis[i];
        global->finais[i]        += local->finais[i];
        global->titulos[i]       += local->titulos[i];
    }
}

```

---

## 4. Fluxo do `main.c`

### Versão base (serial)

```c
#include <stdio.h>
#include "teams.h"
#include "sim.h"
#include "stats.h"
#include "random_utils.h"

int main(int argc, char *argv[]) {
    int N = 1000; // número de simulações (pode ler da linha de comando)
    if (argc > 1) {
        N = atoi(argv[1]);
    }

    Time times[NUM_TIMES];
    Estatisticas estat_global, estat_local;

    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);
    zerar_estatisticas(&estat_global);

    init_random_seed(0);

    for (int i = 0; i < N; i++) {
        zerar_estatisticas(&estat_local);
        simular_campeonato(times, &estat_local);
        acumular_estatisticas(&estat_global, &estat_local);
    }

    // imprimir probabilidades
    for (int i = 0; i < NUM_TIMES; i++) {
        double p_titulo = (double)estat_global.titulos[i] / (double)N * 100.0;
        printf("%s: %.2f%% títulos\n", times[i].nome, p_titulo);
    }

    return 0;
}

```

Depois, esse `main` vira:

- `main_serial.c`
- `main_openmp.c`
- `main_mpi.c`

ou você usa `#ifdef` pra separar versões.

---

## 5. Onde entra OpenMP e MPI

### 🧵 OpenMP

No lugar do `for (int i = 0; i < N; i++)`:

- cada thread terá sua `Estatisticas estat_local_thread;`
- no final, você combina no `estat_global`.

Exemplo simples (esqueleto):

```c
#include <omp.h>

#pragma omp parallel
{
    Estatisticas estat_local_thread;
    zerar_estatisticas(&estat_local_thread);

    #pragma omp for
    for (int i = 0; i < N; i++) {
        simular_campeonato(times, &estat_local_thread);
    }

    #pragma omp critical
    {
        acumular_estatisticas(&estat_global, &estat_local_thread);
    }
}

```

Compilar:

```bash
gcc -fopenmp src/*.c -Iinclude -o champions_omp

```

---

### 📦 MPI

Estratégia:

- Cada processo MPI roda `N_local = N_total / num_procs` simulações;
- Cada processo produz `estat_local`;
- Processo 0 recebe tudo via `MPI_Reduce` (ou `MPI_Gather` + soma manual).

Esqueleto:

```c
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N_total = 1000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N_local = N_total / size;

    Time times[NUM_TIMES];
    Estatisticas estat_local, estat_global;

    carregar_times(times);
    calcular_forcas(times, NUM_TIMES);
    zerar_estatisticas(&estat_local);
    zerar_estatisticas(&estat_global);

    init_random_seed(rank * 1234 + 1);

    for (int i = 0; i < N_local; i++) {
        simular_campeonato(times, &estat_local);
    }

    // aqui você faz um MPI_Reduce em cada vetor de estatísticas
    MPI_Reduce(estat_local.passou_grupos, estat_global.passou_grupos,
               NUM_TIMES, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    // repetir para quartas, semis, finais, titulos...

    if (rank == 0) {
        // imprime probabilidades
    }

    MPI_Finalize();
    return 0;
}

```

Compilar:

```bash
mpicc src/*.c -Iinclude -o champions_mpi

```

---

## 6. Makefile simples

```makefile
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -O2 -Iinclude

SRC = src/main.c src/teams.c src/sim.c src/stats.c src/random_utils.c
OBJ = $(SRC:.c=.o)

all: serial openmp mpi

serial: $(OBJ)$(CC) $(CFLAGS) $(OBJ) -o champions_serial

openmp: $(OBJ)$(CC) $(CFLAGS) -fopenmp $(OBJ) -o champions_openmp

mpi: $(SRC)$(MPICC) $(CFLAGS) $(SRC) -o champions_mpi

clean:
	rm -f src/*.o champions_serial champions_openmp champions_mpi

```