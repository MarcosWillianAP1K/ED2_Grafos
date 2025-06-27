
#pragma once

typedef struct CONFIGURACOES
{
    int *configuracao; // Configuração atual dos discos
} CONFIGURACOES;

typedef struct HANOI
{
    CONFIGURACOES *config; // Todas configurações das torres
    int n_discos;          // Número de discos
    int n_torres;         // Número de torres
} HANOI;


int max_possibilidades_hanoi(int n_discos, int n_torres);

int disco_no_topo(CONFIGURACOES config, int pino_idx, int n_discos);

short int is_valid_hanoi_config(CONFIGURACOES config, int n_discos, int n_torres);

void gerar_configuracoes_hanoi(HANOI *hanoi);

HANOI iniciar_hanoi(int n_discos, int n_torres);

HANOI criar_hanoi(int n_discos, int n_torres);

void liberar_hanoi(HANOI *hanoi);
