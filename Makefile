CC=mpicc
CFLAGS=-O2 -Wall -fopenmp

SRC=src/*.c
OUT=simulador

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	mpirun -np 4 ./$(OUT)

clean:
	rm -f $(OUT)
