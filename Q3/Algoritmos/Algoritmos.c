#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MIN
#include "Algoritmos.h"
#include "../Utilitarios/Utilitarios.h"

#define FLOAT_MIN -0.0000000001 // Valor mínimo para comparação de distâncias

/****************************************
 * *
 * IMPLEMENTAÇÃO: VETOR_MAIOR_CAMINHO *
 * (Ajustado para lógica de maior caminho) *
 ****************************************/

void liberar_vetor_maior_caminho(VETOR_MAIOR_CAMINHO **vetor)
{
    if (vetor != NULL && *vetor != NULL)
    {
        free(*vetor);
        *vetor = NULL;
    }
}


void inicializar_vetor_maior_caminho(VETOR_MAIOR_CAMINHO *vetor, int n_vertices)
{
    for (int i = 0; i < n_vertices; i++)
    {
        vetor[i].vertice_anterior = -1;
        vetor[i].vertice_visitado = 0;
        vetor[i].distancia = INT_MIN; // Sempre inicializa para maior caminho
    }
}


VETOR_MAIOR_CAMINHO *alocar_vetor_maior_caminho(int n_vertices)
{
    VETOR_MAIOR_CAMINHO *vetor = (VETOR_MAIOR_CAMINHO *)malloc(n_vertices * sizeof(VETOR_MAIOR_CAMINHO));
    verificar_alocacao(vetor, "falha ao alocar memória para o vetor de caminho");
    inicializar_vetor_maior_caminho(vetor, n_vertices); // Não passa mais is_longest_path

    return vetor;
}


void imprimir_vetor_maior_caminho(VETOR_MAIOR_CAMINHO *vetor, int n_vertices)
{
    printf("Resultado (Vertice: Distancia, Anterior):\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("  Vertice %d: ", i + 1);
        if (vetor[i].distancia == INT_MIN) // Lógica de verificação de infinito para maior caminho
        {
            printf("Dist: Infinito, Ant: %d\n", vetor[i].vertice_anterior);
        }
        else
            printf("Dist: %lf, Ant: %d\n", vetor[i].distancia, vetor[i].vertice_anterior + 1);
    }
}


void imprimir_caminho(VETOR_MAIOR_CAMINHO *caminho, int vertice_inicial, int vertice_final)
{
    if (caminho != NULL && vertice_inicial > 0 && vertice_final > 0)
    {

        if (caminho[vertice_final - 1].distancia != INT_MIN)
        {

            int quant_vertices = 0;
            int anterior = vertice_final - 1;

            while (anterior != vertice_inicial - 1 && anterior >= 0)
            {
                quant_vertices++;
                anterior = caminho[anterior].vertice_anterior;
            }


            printf("Caminho do vertice %d ao vertice %d:\n", vertice_inicial, vertice_final);

            anterior = vertice_final - 1;

            for (int i = quant_vertices; i >= 0; i--)
            {
                if (anterior != vertice_inicial - 1)
                {
                    printf("%d - ", anterior + 1);                 // Imprime o vértice atual
                    anterior = caminho[anterior].vertice_anterior; // Move para o vértice anterior
                }
                else
                {
                    printf("%d\n", anterior + 1); // Imprime o vértice inicial
                }
            }
        }
        else
        {
            printf("Nao ha caminho do vertice %d ao vertice %d.\n", vertice_inicial, vertice_final);
        }
    }
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: Dijkstra (Maior Caminho) *
 * *
 ****************************************/

VETOR_MAIOR_CAMINHO *dijkstra_maior_caminho(GRAFO *grafo, int vertice_inicial)
{
    VETOR_MAIOR_CAMINHO *vetor_vertices = NULL;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_inicial > 0 && vertice_inicial <= grafo->n_vertices)
    {
        vetor_vertices = alocar_vetor_maior_caminho(grafo->n_vertices); // Não passa mais o segundo parâmetro
        vetor_vertices[vertice_inicial - 1].distancia = 0;

        for (int qtdRelaxamentos = 0; qtdRelaxamentos < grafo->n_vertices - 1; qtdRelaxamentos++)
        {
            float distanciaMaxima = FLOAT_MIN;
            int indiceVerticeAtual = 0;

            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if (!vetor_vertices[i].vertice_visitado && vetor_vertices[i].distancia >= distanciaMaxima)
                {
                    distanciaMaxima = vetor_vertices[i].distancia;
                    indiceVerticeAtual = i;
                }
            }

            vetor_vertices[indiceVerticeAtual].vertice_visitado = 1;

            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if (!vetor_vertices[i].vertice_visitado && grafo->matriz_adjacencia[indiceVerticeAtual][i].bolean)
                {
                    float dist = vetor_vertices[indiceVerticeAtual].distancia + grafo->matriz_adjacencia[indiceVerticeAtual][i].peso_aresta;
                    
                    if (vetor_vertices[indiceVerticeAtual].distancia != FLOAT_MIN && dist > vetor_vertices[i].distancia)
                    {
                        vetor_vertices[i].distancia = dist;
                        vetor_vertices[i].vertice_anterior = indiceVerticeAtual;
                    }
                }
            }
        }
    }

    return vetor_vertices;
}
