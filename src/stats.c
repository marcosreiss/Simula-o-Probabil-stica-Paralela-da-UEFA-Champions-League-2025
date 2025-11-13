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
