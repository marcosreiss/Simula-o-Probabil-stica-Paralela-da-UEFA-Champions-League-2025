# ⚽ Simulador Monte Carlo - UEFA Champions League 2025

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Docker](https://img.shields.io/badge/Docker-2496ED?style=flat&logo=docker&logoColor=white)](https://www.docker.com/)
[![OpenMP](https://img.shields.io/badge/OpenMP-green)](https://www.openmp.org/)
[![MPI](https://img.shields.io/badge/MPI-blue)](https://www.open-mpi.org/)

Simulador probabilístico **Monte Carlo** para a UEFA Champions League 2025, desenvolvido em **C puro** com paralelização via **OpenMP** e **MPI**. O projeto demonstra ganhos reais de performance através de comparações quantitativas entre implementações serial, multi-thread e distribuída.

---

## 🎯 Sobre o Projeto

Este projeto implementa um simulador completo da Champions League capaz de executar **milhões de simulações** para estimar probabilidades de:

- 🏆 Times campeões
- 📊 Avanço às fases eliminatórias (quartas, semis, final)
- 📈 Distribuição de resultados por força do time
- 🎲 Impacto da aleatoriedade e sorteio dos grupos

### 🔬 Modelo Probabilístico

Cada time é caracterizado por 4 atributos (ATK, DEF, FORM, EXP) e os placares são gerados usando **distribuição de Poisson**, simulando comportamento realista de gols em partidas de futebol.

### 📊 Resultados (10 milhões de simulações)

| Métrica                       | Versão Serial           |
| ----------------------------- | ----------------------- |
| **Tempo de execução**         | 119,42 segundos         |
| **Simulações/segundo**        | 83.736                  |
| **Probabilidade do favorito** | Manchester City: 22,12% |

---

## 🚀 Quick Start

### 1️⃣ Pré-requisitos

- [Docker Desktop](https://www.docker.com/products/docker-desktop)
- [VS Code](https://code.visualstudio.com/) + Extensão [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

### 2️⃣ Clone o repositório

```
git clone https://github.com/seu-usuario/simulador-champions-montecarlo.git
cd simulador-champions-montecarlo

```

text

### 3️⃣ Abra no VS Code

```
code .

```

text

### 4️⃣ Reabra no container

**Ctrl+Shift+P** → `Dev Containers: Reopen in Container`

O VS Code irá automaticamente:

- ✅ Buildar a imagem Docker
- ✅ Criar o container com GCC + OpenMP + OpenMPI
- ✅ Montar seu código dentro do ambiente

### 5️⃣ Compile e execute

Versão serial

```
make serial
make run

```

Versão OpenMP (8 threads)

```
make omp
make run-omp

```

Versão MPI (4 processos)

```
make mpi
make run-mpi

```

text

---

## 📁 Estrutura do Projeto

```
simulador-champions-montecarlo/
├── .devcontainer/
│   ├── devcontainer.json      # Configuração VS Code
│   └── Dockerfile             # Ubuntu 22.04 + GCC + MPI
├── include/                   # Headers (.h)
│   ├── teams.h
│   ├── random_utils.h
│   ├── sim.h
│   └── stats.h
├── src/                       # Código fonte (.c)
│   ├── main.c
│   ├── teams.c
│   ├── random_utils.c
│   ├── sim.c
│   └── stats.c
├── docs/                      # Documentação
│   ├── escopo.md
│   ├── modelo-prob.md
│   ├── pipeline.md
│   └── times.md
├── Makefile                   # Build system
└── README.md

```

text

---

## 🛠️ Comandos Úteis

### Compilação

```
make serial   # Compila versão serial
make omp      # Compila versão OpenMP
make mpi      # Compila versão MPI
make clean    # Remove executáveis

```

text

### Execução

Serial com número customizado de simulações

```
./simulador_serial 1000000

```

OpenMP com 16 threads

```
OMP_NUM_THREADS=16 ./simulador_omp 5000000

```

MPI com 8 processos

```
mpirun -np 8 ./simulador_mpi 10000000

```

text

### Debug

```
gdb ./simulador_serial

```

text

---

## 🧪 Pipeline de Simulação

Cada simulação completa da Champions League executa:

1. **Montagem dos potes** → Divisão dos 18 times em 3 potes de 6 times
2. **Sorteio dos grupos** → Embaralhamento e distribuição em 3 grupos
3. **Fase de grupos** → 90 partidas (todos contra todos, ida e volta)
4. **Classificação** → 2 primeiros + 2 melhores terceiros = 8 classificados
5. **Mata-mata** → Quartas → Semifinais → Final
6. **Registro estatístico** → Acumulação dos resultados

### Algoritmo de Geração de Gols

Utiliza **distribuição de Poisson** com λ (lambda) calculado dinamicamente:

```
λ = (ATK_atacante + (100 - DEF_defensor)) / 100 × (FORM / 100) × 1.5
gols = poisson(λ)

```

text

---

## 📈 Roadmap

### ✅ Fase 1 - Implementação Serial (Concluída)

- [x] Modelo probabilístico com Poisson
- [x] Pipeline completo de simulação
- [x] Sistema de estatísticas
- [x] Medição de tempo com `clock_gettime()`
- [x] 10 milhões de simulações validadas

### 🚧 Fase 2 - Paralelização OpenMP (Em Progresso)

- [ ] Paralelização do loop Monte Carlo
- [ ] Sincronização com `#pragma omp critical`
- [ ] Testes de speedup (1, 2, 4, 8, 16 threads)
- [ ] Análise de eficiência paralela

### 📋 Fase 3 - Paralelização MPI (Planejada)

- [ ] Distribuição de simulações entre processos
- [ ] Agregação de resultados com `MPI_Reduce`
- [ ] Testes em cluster distribuído
- [ ] Comparação de desempenho serial vs OpenMP vs MPI

### 🎯 Fase 4 - Refinamentos (Futuro)

- [ ] Calibração avançada dos atributos dos times
- [ ] Modelo híbrido (MPI + OpenMP)
- [ ] Inclusão de fator casa/visitante
- [ ] Visualização gráfica dos resultados
- [ ] API REST para simulações sob demanda

---

## 🤝 Como Contribuir

Contribuições são **muito bem-vindas**! Este é um projeto educacional e colaborativo.

### 🌟 Áreas de Contribuição

- 🐛 **Correção de bugs** na lógica de simulação
- ⚡ **Otimizações de performance** no código serial
- 🧪 **Implementação de testes unitários**
- 📊 **Refinamento do modelo probabilístico**
- 📈 **Análise de dados e visualizações**
- 📝 **Melhorias na documentação**
- 🏆 **Atualização dos dados dos times**

### 📝 Processo de Contribuição

1. **Fork** o repositório
2. Crie uma branch para sua feature (`git checkout -b feature/minha-contribuicao`)
3. Commit suas mudanças (`git commit -m 'feat: adiciona nova funcionalidade'`)
4. Push para a branch (`git push origin feature/minha-contribuicao`)
5. Abra um **Pull Request** detalhando suas alterações

### 💡 Ideias para PRs

- Implementar variações do modelo probabilístico
- Adicionar suporte a outros campeonatos (Copa do Mundo, Libertadores)
- Criar scripts de análise estatística em Python
- Dockerizar versão para produção (sem VS Code)
- Adicionar CI/CD com GitHub Actions
- Implementar logging estruturado

---

## 📚 Documentação Técnica

- 📋 Escopo do Projeto — `docs/escopo.md`
- 🎲 Modelo Probabilístico — `docs/modelo-prob.md`
- 🔄 Pipeline de Simulação — `docs/pipeline.md`
- ⚽ Dados dos Times — `docs/times.md`

---

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como trabalho da disciplina de **Programação Paralela** ministrada pelo Prof. Omar Andres Carmona Cortes, com foco em:

- Comparação quantitativa serial vs paralelo
- Análise de speedup e eficiência
- Aplicação prática de OpenMP e MPI
- Problemas do tipo _embarrassingly parallel_

---

## 📊 Benchmarks

| Versão                 | Simulações | Tempo   | Sims/seg | Speedup |
| ---------------------- | ---------- | ------- | -------- | ------- |
| **Serial**             | 10M        | 119,42s | 83.736   | 1,00x   |
| **OpenMP (8 threads)** | 10M        | TBD     | TBD      | TBD     |
| **MPI (8 processos)**  | 10M        | TBD     | TBD      | TBD     |

_Benchmarks realizados em: Docker Ubuntu 22.04, GCC 11.4, CPU AMD/Intel (especificar)_

---

## 🛡️ Licença

Este projeto está sob a licença MIT. Veja o arquivo **LICENSE** para mais detalhes.

---

## 👨‍💻 Autor

**Marcos Vinicius Santos da Silva Reis**

- GitHub: [https://github.com/seu-usuario](https://github.com/marcosreiss)
- LinkedIn: [https://linkedin.com/in/seu-perfil](https://www.linkedin.com/in/marcosvinireis/)

---

## 🙏 Agradecimentos

- Prof. Omar Andres Carmona Cortes pela orientação
- Comunidade OpenMP e Open MPI
- Contribuidores do projeto

---

## ⭐ Gostou do projeto?

Se este projeto te ajudou de alguma forma, considere dar uma ⭐ no repositório! Isso incentiva o desenvolvimento contínuo e ajuda outros estudantes a encontrarem este recurso.

**Contribuições, issues e sugestões são sempre bem-vindas!** 🚀

---

Desenvolvido com ❤️ e ☕ por estudantes de Computação
