CC = gcc
CFLAGS = -Wall -O3 -lm -fopenmp
SRC_DIR = src
INC_DIR = include

# Arquivos fonte
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/teams.c \
          $(SRC_DIR)/random_utils.c \
          $(SRC_DIR)/sim.c \
          $(SRC_DIR)/stats.c

# Executáveis
TARGET_SERIAL = simulador_serial
TARGET_OMP = simulador_omp
TARGET_MPI = simulador_mpi

# Compilação da versão serial
serial: $(SOURCES)
	$(CC) -I$(INC_DIR) $(SOURCES) -o $(TARGET_SERIAL) -lm

# Compilação da versão OpenMP
omp: $(SOURCES)
	$(CC) -I$(INC_DIR) $(SOURCES) -o $(TARGET_OMP) -fopenmp -lm

# Compilação da versão MPI
mpi: $(SOURCES)
	mpicc -I$(INC_DIR) $(SOURCES) -o $(TARGET_MPI) -lm

# Rodar versão serial
run: serial
	./$(TARGET_SERIAL) 10000000

# Rodar versão OpenMP
run-omp: omp
	./$(TARGET_OMP) 10000

# Rodar versão MPI
run-mpi: mpi
	mpirun -np 4 ./$(TARGET_MPI) 10000

# Limpar executáveis
clean:
	rm -f $(TARGET_SERIAL) $(TARGET_OMP) $(TARGET_MPI)

# Alvo padrão
all: serial

.PHONY: all serial omp mpi run run-omp run-mpi clean
