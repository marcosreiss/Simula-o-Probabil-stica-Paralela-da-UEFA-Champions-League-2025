# 🎲 1. O que é o modelo probabilístico?

É simplesmente a forma como decidimos:

- a força do time,
- a probabilidade de marcar gols,
- a probabilidade de sofrer gols,
- a chance de vencer, empatar ou perder,
- como gerar placares (0x0, 2x1, 3x2 etc.)

A chave: **cada jogo deve ser gerado por uma equação simples**, baseada nos atributos dos times.

Não usamos machine learning.

Usamos **probabilidade + estatística básica + aleatoriedade**.

---

# 🧩 2. Quais atributos vamos usar?

Para não complicar demais, a melhor abordagem é usar **4 atributos principais**, todos normalizados entre 0 e 100:

### **1. Ataque (ATK)**

– Capacidade de criar chances.

– Derivado de gols marcados, xG, desempenho ofensivo.

### **2. Defesa (DEF)**

– Capacidade de evitar chances.

– Derivado de gols sofridos, clean sheets etc.

### **3. Forma Atual (FORM)**

– Performance recente (últimos 5–10 jogos).

– Pode ser importado diretamente do Sofascore (nota média).

### **4. Experiência/Historicidade (EXP)**

– Tradição em Champions League.

– Influencia levemente em mata-mata.

**4 atributos → 1 força geral.**

Você combina eles em um único valor:

```
FORCA = 0.35*ATK + 0.35*DEF + 0.20*FORM + 0.10*EXP

```

Esse valor define a **qualidade geral do time**.

---

# ⚽ 3. Como gerar probabilidades de gols? (modelo simples e funcional)

Podemos usar um modelo **Poisson simplificado**, muito comum em futebol.

### Para cada time:

```
média_gols = (FORCA_ATK / 100) * 1.8    // 1.8 é média europeia
média_sofridos = (FORCA_DEF / 100) * 1.6

```

A força do adversário modula isso:

```
media_final = (media_gols * 0.7) + (media_sofridos_do_oponente * 0.3)

```

Depois usamos **Poisson** para gerar placares:

```
gols_time = poisson(media_final)

```

Poisson é simples de implementar em C.

---

# 🎯 4. Como transformar isso em probabilidade de vitória?

Quando simulamos **gols gerados**, o placar já define:

- vitória
- empate
- derrota

Então não precisamos calcular vitória diretamente.

A probabilidade emerge do modelo.

---

# 🔢 5. Como implementar isso em C?

Cada time pode ser representado por uma struct:

```c
typedef struct {
    char nome[50];
    double atk;   // 0–100
    double def;   // 0–100
    double form;  // 0–100
    double exp;   // 0–100
    double forca; // calculado
} Time;

```

A força é calculada:

```c
time.forca = 0.35*time.atk + 0.35*time.def + 0.20*time.form + 0.10*time.exp;

```

---

# ⚙️ 6. Simulação de um jogo (macro)

Para cada time:

1. calcular média de gols esperados com base nas forças
2. gerar gols via Poisson
3. retornar placar

---

# 🧪 7. Por que esse modelo funciona?

- Simples
- Realista
- Escala bem para simular milhares de jogos
- Fácil de paralelizar
- Permite comparar times com boa fidelidade
- Evita fórmulas estatísticas avançadas que complicariam C
- Comporta bem os dados do Sofascore, que já vêm normalizados

Esse é o modelo usado por **centenas de simuladores de futebol**, inclusive modelos acadêmicos simples.

---

# 🔥 8. Opcional (posterior): ajustar pesos

Você pode ajustar pesos:

- para dar mais importância ao ataque,
- para equilibrar defesa,
- para deixar a competição mais ou menos aleatória.