#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Semente aleatória diferente por processo
    srand(time(NULL) + world_rank);

    // Cada processo MPI fará uma "simulação paralela" com OpenMP
    int gols_local = 0;

    #pragma omp parallel reduction(+:gols_local)
    {
        int thread_id = omp_get_thread_num();
        unsigned int seed = time(NULL) + thread_id + world_rank * 100;

        // Exemplo: 1000 iterações Monte Carlo por thread
        for (int i = 0; i < 1000; i++) {
            double r = rand_r(&seed) / (double)RAND_MAX;

            // Suponha que a chance de gol seja 10%
            if (r < 0.10) {
                gols_local++;
            }
        }
    }

    // Reduce: soma todos os resultados dos processos
    int gols_total = 0;
    MPI_Reduce(&gols_local, &gols_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("=== Teste MPI + OpenMP ===\n");
        printf("Total de processos MPI: %d\n", world_size);
        printf("Gols simulados (Monte Carlo simplificado): %d\n", gols_total);
        printf("Ambiente funcionando corretamente! 🎉\n");
    }

    MPI_Finalize();
    return 0;
}
