## 🧾 Lista de 18 times + atributos (ATK, DEF, FORM, EXP)

Valores todos entre **0 e 100**, e você pode ajustar depois se quiser deixar algum time mais forte/fraco.

> Campos:
> 
> - **ATK** = ataque
> - **DEF** = defesa
> - **FORM** = forma recente
> - **EXP** = experiência/tradição em Champions

### 🏆 Times mais fortes

1. **Manchester City**
    - ATK: 95
    - DEF: 90
    - FORM: 92
    - EXP: 85
2. **Real Madrid**
    - ATK: 93
    - DEF: 88
    - FORM: 90
    - EXP: 100
3. **Bayern de Munique**
    - ATK: 92
    - DEF: 88
    - FORM: 88
    - EXP: 95
4. **PSG**
    - ATK: 91
    - DEF: 85
    - FORM: 87
    - EXP: 80
5. **Liverpool**
    - ATK: 90
    - DEF: 87
    - FORM: 86
    - EXP: 90
6. **Barcelona**
    - ATK: 89
    - DEF: 84
    - FORM: 85
    - EXP: 95

---

### 🔝 Times fortes, mas um pouco abaixo dos tops

1. **Arsenal**
    - ATK: 88
    - DEF: 84
    - FORM: 87
    - EXP: 75
2. **Inter de Milão**
    - ATK: 86
    - DEF: 88
    - FORM: 85
    - EXP: 85
3. **Atlético de Madrid**
    - ATK: 83
    - DEF: 89
    - FORM: 83
    - EXP: 85
4. **Juventus**
    - ATK: 84
    - DEF: 86
    - FORM: 80
    - EXP: 90
5. **Milan**
    - ATK: 84
    - DEF: 82
    - FORM: 82
    - EXP: 88
6. **Borussia Dortmund**
    - ATK: 85
    - DEF: 80
    - FORM: 84
    - EXP: 75

---

### ⚖️ Times competitivos / médios

1. **RB Leipzig**
    - ATK: 82
    - DEF: 80
    - FORM: 83
    - EXP: 65
2. **Napoli**
    - ATK: 84
    - DEF: 78
    - FORM: 82
    - EXP: 70
3. **Porto**
    - ATK: 80
    - DEF: 80
    - FORM: 81
    - EXP: 80
4. **Benfica**
    - ATK: 81
    - DEF: 79
    - FORM: 80
    - EXP: 78
5. **Newcastle**
    - ATK: 82
    - DEF: 78
    - FORM: 79
    - EXP: 60
6. **Bayer Leverkusen**
    - ATK: 83
    - DEF: 79
    - FORM: 86
    - EXP: 65

---

## 🧱 Como isso vira código em C

Lembra da nossa `struct Time`? Podemos preencher um array estático:

```c
typedef struct {
    char nome[50];
    double atk;   // 0–100
    double def;
    double form;
    double exp;
    double forca; // calculada depois
} Time;

Time times[18] = {
    {"Manchester City", 95, 90, 92, 85, 0},
    {"Real Madrid",     93, 88, 90, 100, 0},
    {"Bayern Munich",   92, 88, 88, 95, 0},
    {"PSG",             91, 85, 87, 80, 0},
    {"Liverpool",       90, 87, 86, 90, 0},
    {"Barcelona",       89, 84, 85, 95, 0},
    {"Arsenal",         88, 84, 87, 75, 0},
    {"Inter Milan",     86, 88, 85, 85, 0},
    {"Atletico Madrid", 83, 89, 83, 85, 0},
    {"Juventus",        84, 86, 80, 90, 0},
    {"AC Milan",        84, 82, 82, 88, 0},
    {"Borussia Dortmund", 85, 80, 84, 75, 0},
    {"RB Leipzig",      82, 80, 83, 65, 0},
    {"Napoli",          84, 78, 82, 70, 0},
    {"Porto",           80, 80, 81, 80, 0},
    {"Benfica",         81, 79, 80, 78, 0},
    {"Newcastle",       82, 78, 79, 60, 0},
    {"Bayer Leverkusen",83, 79, 86, 65, 0}
};

```

Depois, você roda uma função para calcular a **força geral** de cada time:

```c
void calcular_forca(Time *t) {
    t->forca = 0.35 * t->atk +
               0.35 * t->def +
               0.20 * t->form +
               0.10 * t->exp;
}

void inicializar_forcas(Time times[], int n) {
    for (int i = 0; i < n; i++) {
        calcular_forca(&times[i]);
    }
}

```