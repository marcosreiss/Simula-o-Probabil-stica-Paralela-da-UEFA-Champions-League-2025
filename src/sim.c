void simular_campeonato(Time times[], Estatisticas *estat_local) {
    Time pote1[6], pote2[6], pote3[6];
    Grupo grupos[NUM_GRUPOS];
    Time *classificados_quartas[8];
    Time *classificados_semis[4];
    Time *finalistas[2];
    Time *campeao = NULL;

    montar_potes(times, pote1, pote2, pote3);
    sortear_grupos(pote1, pote2, pote3, grupos);
    simular_fase_grupos(grupos, classificados_quartas);
    simular_mata_mata(classificados_quartas,
                      &classificados_quartas, // ou null se não quiser detalhar
                      &classificados_semis,
                      &finalistas,
                      &campeao);

    registrar_resultado_torneio(estat_local,
                                classificados_quartas,
                                classificados_semis,
                                finalistas,
                                campeao);
}
