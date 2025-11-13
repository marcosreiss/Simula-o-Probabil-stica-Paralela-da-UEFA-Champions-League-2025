# 🏗️ **PIPELINE COMPLETO DA SIMULAÇÃO DA CHAMPIONS (uma edição)**

A simulação de uma Champions completa envolve 6 grandes etapas:

---

# **1️⃣ Preparação inicial**

Antes de simular qualquer coisa, o programa:

### 1.1 Carrega os times

- Array com os 18 times
- Cada time tem atributos ATK, DEF, FORM, EXP
- Calcula o valor de **força geral**

### 1.2 Separa os 3 potes

Já deixamos isso pronto para o sorteio.

### 1.3 Inicializa estado da simulação

- Zerar estatísticas de classificação
- (Opcional) inicializar gerador de números aleatórios

---

# **2️⃣ Sorteio dos grupos**

### 2.1 Embaralha cada pote

- Usando Fisher-Yates (função `embaralhar()`)

### 2.2 Distribui nos 3 grupos

- 6 times por grupo
- 2 times de cada pote por grupo

Resultado:

```
Grupo A: 6 times
Grupo B: 6 times
Grupo C: 6 times

```

---

# **3️⃣ Simulação da fase de grupos**

Cada grupo terá:

- **6 times**
- **cada time joga 10 partidas** (ida + volta)
- total de **30 partidas** por grupo
- total geral: **90 partidas**

### 3.1 Obter tabela inicial do grupo

- Zerar pontos, gols pró, gols contra

### 3.2 Loop de partidas do grupo

Para cada par de times (i, j):

1. Simula o jogo **ida**
    - usa o modelo probabilístico
    - registra placar
    - atualiza pontos e tabela
2. Simula o jogo **volta**
    - mesmo processo

### 3.3 Classificação final do grupo

Ordena por:

1. Pontos
2. Saldo de gols
3. Gols marcados
4. (Opcional) força geral como critério final

### 3.4 Seleção de classificados

- 1º e 2º de cada grupo → avançam
- Os 3º colocados vão para disputa por vaga
- Os **2 melhores terceiros colocados** → avançam

Total: **8 classificados**

---

# **4️⃣ Simulação do mata-mata**

Temos:

- Quartas (8 times)
- Semifinais (4 times)
- Final (2 times)

### 4.1 Quartas de final

- Sorteio simples: embaralhar os 8 classificados
- Confrontos: 1×2, 3×4, 5×6, 7×8
- Jogar ida e volta
- Critério:
    - soma dos gols
    - se empate → pênaltis (opcional: escolher aleatório)

### 4.2 Semifinais

Mesma lógica das quartas

(ida + volta + desempate)

### 4.3 Final

Jogo único

Se empate → pênaltis

### 4.4 Campeão é definido

---

# **5️⃣ Acumular estatísticas finais**

Após **uma simulação completa**, você contabiliza:

Para cada time:

- incrementa `passou_fase_grupos`
- incrementa `quartas`
- incrementa `semis`
- incrementa `final`
- incrementa `titulos`

E também pode guardar:

- média de gols marcados
- média de gols sofridos
- distribuição de eliminações por fase

Esses contadores ficam em uma struct de estatísticas.

---

# **6️⃣ Loop Monte Carlo (múltiplas simulações)**

A parte **central do paralelismo**.

### Loop serial:

```
for(sim = 0; sim < N; sim++) {
    simular_campeonato();
}

```

### Loop com OpenMP:

```
#pragma omp parallel for reduction(+:estatisticas)
for(sim = 0; sim < N; sim++) {
    simular_campeonato();
}

```

### Loop com MPI:

- Cada processo recebe um bloco de simulações:
    
    ```
    total_processos = P
    sims_por_processo = N / P
    
    ```
    
- Cada processo acumula estatísticas locais
- `MPI_Reduce` combina tudo no processo 0

---

# 🧩 **Em resumo, o pipeline completo é:**

```
1. Preparação dos times
2. Sorteio dos grupos
3. Fase de grupos
4. Mata-mata
5. Atualização das estatísticas
6. Repetição via Monte Carlo (Seq / OpenMP / MPI)
7. Output final das probabilidades

```
---