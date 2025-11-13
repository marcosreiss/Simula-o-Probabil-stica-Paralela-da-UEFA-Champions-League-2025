# **Simulador de Partidas de Futebol (Monte Carlo)**

Ambiente de desenvolvimento em **C** com **OpenMP + OpenMPI**, configurado para rodar no **VS Code Dev Containers**, garantindo isolamento, reprodutibilidade e facilidade de uso.

---

## **1. Estrutura do Projeto**

```
Simulador-Partidas-Futebol-MonteCarlo/
 ├─ .devcontainer/
 │   ├─ devcontainer.json        → Configuração do VS Code
 │   └─ Dockerfile               → Ambiente Linux com GCC + MPI
 ├─ src/                         → Coloque seus .c aqui
 ├─ Makefile                     → Compilar / rodar
 └─ README.md

```

---

## **2. Ambiente Docker**

O ambiente roda dentro de um container baseado em **Ubuntu 22.04**, contendo:

- gcc e g++
- make
- openmpi-bin + libopenmpi-dev
- suporte completo a OpenMP
- mpicc, mpirun, mpiexec
- gdb para debug

O código **não fica dentro do container**, apenas o ambiente.

Toda a pasta `src/` é montada automaticamente dentro do container.

---

## **3. Como usar (VS Code)**

### **1. Instale:**

- Docker Desktop
- Extensão “Dev Containers” no VS Code

### **2. Abra o projeto no VS Code:**

```
File → Open Folder → Simulador-Partidas-Futebol-MonteCarlo

```

### **3. Reabra no container:**

```
CTRL + SHIFT + P → Dev Containers: Reopen in Container

```

O VS Code irá:

1. Buildar a imagem
2. Criar o container
3. Abrir o workspace dentro do container

---

## **4. Compilação e Execução (Makefile)**

### **Compilar:**

```
make

```

### **Rodar (4 processos MPI):**

```
make run

```

Ou manualmente:

```
mpirun -np 4 ./simulador

```

### **Limpar binários:**

```
make clean

```

---

## **5. Sobre MPI + OpenMP**

Este ambiente permite desenvolvimento com:

### **✔ MPI – Paralelismo distribuído**

Divide o trabalho entre vários processos independentes.

Comandos principais:

- `mpicc` para compilar
- `mpirun` ou `mpiexec` para executar

---

### **✔ OpenMP – Paralelismo por threads**

Usado dentro de loops paralelos:

```c
#pragma omp parallel

```

O GCC já vem com suporte usando:

```
-fopenmp

```

(que já está incluído no Makefile)

---

### **✔ Híbrido (MPI + OpenMP)**

Usando múltiplos processos MPI, cada um com múltiplas threads OpenMP.

Perfeito para simulações Monte Carlo.

---

## **6. Fluxo de Trabalho Recomendado**

### **1. Editar seus arquivos em:**

```
src/

```

### **2. Compilar:**

```
make

```

### **3. Executar:**

```
mpirun -np 4 ./simulador

```

### **4. Debug (GDB):**

```
gdb ./simulador

```

---

## **7. Ambiente Pronto**

Agora você possui:

- Ambiente isolado via Docker
- Compilador moderno (GCC)
- Suporte completo para OpenMP
- Suporte completo para OpenMPI
- Persistência garantida
- Configuração automatizada no VS Code
- Makefile simples e funcional

Perfeito para desenvolver o **Simulador de Partidas de Futebol usando o Método de Monte Carlo**.
