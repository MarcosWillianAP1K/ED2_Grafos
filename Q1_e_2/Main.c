#include <stdio.h>
#include <stdlib.h>
#include "Hanoi/Hanoi.h"
#include "Algoritmos/Algoritmos.h"

#define N_PINOS 3 // Número de pinos (torres) para a Torre de Hanói
#define N_DISCOS 2 // Número de discos para a Torre de Hanói


short int aresta_hanoi(const CONFIGURACOES v1, const CONFIGURACOES v2)
{
    short int retorno = 1; // Assume que a aresta é válida

    int diff_count = 0;
    int disco_movido_idx = -1; // Índice do disco que mudou de pino
    int pino_origem = -1;
    int pino_destino = -1;

    // Verifica se apenas um disco se moveu
    for (int i = 0; i < N_DISCOS; ++i)
    {
        if (v1.configuracao[i] != v2.configuracao[i])
        {
            diff_count++;
            disco_movido_idx = i;
            pino_origem = v1.configuracao[i];
            pino_destino = v2.configuracao[i];
        }
    }

    if (diff_count != 1)
    {
        retorno = 0; // Mais de um disco se moveu ou nenhum disco se moveu
    }

    // Verifica se o disco movido é o disco do topo do pino de origem
    if (disco_movido_idx != disco_no_topo(v1, pino_origem, N_DISCOS))
    {
        retorno = 0; // O disco movido não era o disco do topo do pino de origem
    }

    // Verifica se o disco movido é colocado sobre um disco maior (ou pino vazio) no pino de destino
    for (int i = 0; i < N_DISCOS; ++i)
    {
        if (i != disco_movido_idx && v2.configuracao[i] == pino_destino)
        {
            // Se outro disco 'i' está no pino de destino de v2
            if (i < disco_movido_idx)
            {             // Se esse outro disco 'i' (menor índice = menor disco) é MENOR que o disco que foi movido
                retorno = 0; // Movimento inválido: disco menor sobre disco maior
            }
        }
    }
    
    return retorno;
}


void construir_grafo_hanoi(GRAFO *grafo, HANOI *hanoi)
{
    // Inicializa o grafo com as configurações da Torre de Hanói
    for (int i = 0; i < max_possibilidades_hanoi(hanoi->n_discos, hanoi->n_torres); i++)
    {
        for (int j = 0; j < max_possibilidades_hanoi(hanoi->n_discos, hanoi->n_torres); j++)
        {
            // Se o movimento é válido, adiciona uma aresta com peso 1
            if (aresta_hanoi(hanoi->config[i], hanoi->config[j]))
            {
                criar_aresta(grafo, i + 1, j + 1, 1); // Adiciona aresta com peso 1
            }

        }
    }
}


int main()
{
    //Inicia o grafo
    HANOI hanoi = criar_hanoi(N_DISCOS, N_PINOS);

    printf("Torre de Hanoi com %d discos e %d pinos.\nCom %d possibilidades de configuracoes.\n", hanoi.n_discos, hanoi.n_torres, max_possibilidades_hanoi(N_DISCOS, N_PINOS));

    GRAFO grafo_hanoi = criar_grafo(0, 0, max_possibilidades_hanoi(N_DISCOS, N_PINOS)); // Ponderado, Direcionado

    printf("\nGrafo de Hanoi criado com %d vertices.\n\n", grafo_hanoi.n_vertices);

    construir_grafo_hanoi(&grafo_hanoi, &hanoi);

    imprimir_matriz_grafo(&grafo_hanoi);



    liberar_grafo(&grafo_hanoi);
    liberar_hanoi(&hanoi);

    return 0;
}