# **📘 ESCOPO DO PROJETO — Simulação Probabilística Paralela da UEFA Champions League 2025**

## **1. Introdução**

Este projeto tem como objetivo desenvolver um sistema capaz de simular diversas edições completas da UEFA Champions League 2025 utilizando técnicas de paralelismo com **OpenMP** e **MPI**.

A abordagem será baseada em um modelo **probabilístico Monte Carlo**, permitindo executar milhares de simulações independentes do torneio para estimar o desempenho de cada time em diferentes cenários.

O projeto será implementado inteiramente em **C**, primeiro na forma **serial**, e posteriormente com paralelização por **threads** (OpenMP) e por **processos distribuídos** (MPI), demonstrando ganhos reais de performance.

---

## **2. Justificativa**

Simular uma única edição da Champions League é uma tarefa computacionalmente simples. No entanto, para estimar probabilidades como:

- chances de um time ser campeão,
- chances de avançar às fases eliminatórias,
- distribuição de resultados,
- probabilidade de combinações específicas,

é necessário realizar **milhares ou milhões de simulações** completas do torneio.

Esse volume de processamento torna-se rapidamente inviável em execução sequencial.

Por outro lado, cada simulação é **independente**, caracterizando um problema do tipo *“embarrassingly parallel”*, ideal para aplicação de **OpenMP** e **MPI**. Isso permite:

- dividir a carga de simulações entre múltiplas threads,
- distribuir simulações entre vários processos,
- obter ganhos de performance quase lineares.

Assim, o projeto justifica plenamente a necessidade de paralelismo.

---

## **3. Objetivos**

### **3.1 Objetivo Geral**

Construir um simulador completo da Champions League 2025 e executar múltiplas edições do torneio de forma paralela, analisando o impacto do paralelismo no desempenho.

### **3.2 Objetivos Específicos**

1. Desenvolver um modelo probabilístico para representar os times e partidas.
2. Implementar a simulação completa de uma edição do torneio.
3. Construir a versão **serial** da simulação.
4. Paralelizar o código usando **OpenMP** (threads).
5. Paralelizar o código usando **MPI** (processos distribuídos).
6. Medir e comparar o desempenho das três versões (serial, OpenMP, MPI).
7. Gerar estatísticas agregadas das simulações (probabilidades de classificação e título).

---

## **4. Escopo Funcional**

O sistema realizará:

### **4.1 Modelagem dos Times**

- Representação dos clubes participantes da Champions 2025 com atributos como:
    - força ofensiva
    - força defensiva
    - força geral
    - forma recente
    - histórico em competições europeias
- Esses valores podem vir de fontes públicas (ex.: Sofascore) ou serem estimados manualmente.

### **4.2 Simulação do Sorteio**

Implementação das regras reais do sorteio da Champions:

- divisão dos times em potes;
- impedimentos (times do mesmo país não se enfrentam);
- sorteio dos grupos A–H;

### **4.3 Fase de Grupos**

- Simulação de todos os 96 jogos da fase de grupos;
- Cálculo de resultados baseado em probabilidades derivadas dos atributos dos times;
- Geração da tabela de classificação;
- Aplicação de critérios simples de desempate.

### **4.4 Fase Eliminatória**

- Sorteio das oitavas mantendo restrições oficiais;
- Simulação de jogos de ida e volta;
- Quartas, semifinais e final.

### **4.5 Múltiplas Simulações**

- Execução de milhares de edições completas do torneio;
- Registro de estatísticas como:
    - % de times que avançam às oitavas,
    - % de semifinais, finais,
    - probabilidade de título,
    - distribuição de gols e placares.

---

## **5. Escopo Técnico**

### **5.1 Implementação Serial**

Versão base do sistema, executando uma simulação por vez.

### **5.2 Implementação com OpenMP**

- Paralelização do loop de simulações.
- Compartilhamento de estruturas de resultados.
- Reduções paralelas.

### **5.3 Implementação com MPI**

- Distribuição das simulações entre processos.
- Coleta dos resultados via `MPI_Reduce`.
- Execução com diferentes números de processos.

---

## **6. Métricas e Avaliação de Desempenho**

Serão avaliados:

- Tempo total de execução (serial vs OpenMP vs MPI).
- Speedup e eficiência paralela.
- Escalabilidade com aumento do número de threads/processos.
- Impacto do paralelismo no volume de simulações realizadas.

---

## **7. Resultados Esperados**

- A simulação probabilística do torneio deve retornar estatísticas coerentes com o desempenho real dos times.
- A versão paralela deve apresentar **aceleração significativa** comparada à versão sequencial.
- O projeto deve demonstrar claramente a adequação do problema ao uso de paralelismo.

---

## **8. Conclusão**

O projeto integra três áreas: **modelagem probabilística**, **simulação esportiva** e **computação paralela**, oferecendo um estudo completo sobre desempenho e escalabilidade. A escolha da Champions League torna o trabalho mais interessante e proporciona uma aplicação concreta para técnicas avançadas de paralelismo usando C, OpenMP e MPI.