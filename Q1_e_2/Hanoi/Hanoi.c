#include <stdio.h>
#include <stdlib.h>
#include "Hanoi.h"
#include "../Utilitarios/Utilitarios.h"
#include <math.h>

int max_possibilidades_hanoi(int n_discos, int n_torres)
{
    return (int)pow(n_torres, n_discos);
}

int disco_no_topo(CONFIGURACOES config, int pino_idx, int n_discos)
{
    int topo_disco = n_discos; // n_discos significa que nenhum disco está no pino (maior que qualquer disco válido 0..n_discos-1)
    for (int i = 0; i < n_discos; ++i)
    {
        if (config.configuracao[i] == pino_idx)
        { // Se o disco 'i' está no pino 'pino_idx'
            if (topo_disco == n_discos || i < topo_disco)
            { // Se for o primeiro disco encontrado ou um menor
                topo_disco = i;
            }
        }
    }
    return topo_disco;
}

// Verifica se uma configuração é válida (disco menor sempre sobre disco maior)
short int is_valid_hanoi_config(CONFIGURACOES config, int n_discos, int n_torres)
{
    short int retorno = 1; // Assume que a configuração é válida

    for (int pino_idx = 1; pino_idx <= n_torres; ++pino_idx)
    {
        int maior_disco_na_base = -1; // representa o disco mais baixo/maior

        for (int i = 0; i < n_discos; ++i)
        {
            if (config.configuracao[i] == pino_idx)
            {
                // Se encontramos um disco 'i' no pino 'pino_idx'
                // e já temos um disco na base e o disco atual 'i' é maior que a base
                if (maior_disco_na_base != -1 && i > maior_disco_na_base)
                {
                    retorno = 0; // Configuração inválida
                }
                maior_disco_na_base = i; // Atualiza o disco mais baixo (maior disco encontrado até agora)
            }
        }
    }

    return retorno; // Configuração válida
}

// Gera todas as Pinos^N combinações possíveis e armazena em um array de HANOI
void gerar_configuracoes_hanoi(HANOI *hanoi)
{
    for (int i = 0; i < max_possibilidades_hanoi(hanoi->n_discos, hanoi->n_torres); ++i)
    {
        int num = i;
        for (int j = 0; j < hanoi->n_discos; ++j)
        {
            hanoi->config[i].configuracao[j] = (num % hanoi->n_torres) + 1; // Mapeia 0,1,2 para 1,2,3 (pinos)
            num /= hanoi->n_torres;
        }
    }
}

HANOI iniciar_hanoi(int n_discos, int n_torres)
{
    HANOI hanoi;

    hanoi.n_discos = n_discos;
    hanoi.n_torres = n_torres;

    int n_de_possibilidades = max_possibilidades_hanoi(n_discos, n_torres);

    hanoi.config = (CONFIGURACOES *)malloc(n_de_possibilidades * sizeof(CONFIGURACOES));

    verificar_alocacao(hanoi.config, "falha ao alocar memória para as torres");

    for (int i = 0; i < n_de_possibilidades; i++)
    {
        hanoi.config[i].configuracao = (int *)calloc(1, n_discos * sizeof(int));

        verificar_alocacao(hanoi.config[i].configuracao, "falha ao alocar memória para a configuração dos discos");
    }

    return hanoi;
}

HANOI criar_hanoi(int n_discos, int n_torres)
{
    HANOI hanoi = iniciar_hanoi(n_discos, n_torres);

    gerar_configuracoes_hanoi(&hanoi);

    return hanoi;
}

void liberar_hanoi(HANOI *hanoi)
{
    if (hanoi != NULL)
    {
        for (int i = 0; i < max_possibilidades_hanoi(hanoi->n_discos, hanoi->n_torres); i++)
        {
            free(hanoi->config[i].configuracao);
        }
        free(hanoi->config);
        hanoi->config = NULL;
        hanoi->n_discos = 0;
        hanoi->n_torres = 0;
    }
}