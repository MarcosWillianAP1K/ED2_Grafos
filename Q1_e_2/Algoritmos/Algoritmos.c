#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX
#include "Algoritmos.h"
#include "../Utilitarios/Utilitarios.h"

/****************************************
 * *
 * IMPLEMENTAÇÃO: vetor_caminho_minimo *
 * *
 ****************************************/

void liberar_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO **vetor)
{
    if (vetor != NULL && *vetor != NULL)
    {
        free(*vetor);
        *vetor = NULL;
    }
}

void inicializar_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO *vetor, int n_vertices)
{
    for (int i = 0; i < n_vertices; i++)
    {
        vetor[i].vertice_anterior = -1;
        vetor[i].vertice_visitado = 0;
        vetor[i].distancia = INT_MAX;
    }
}

VETOR_CAMINHO_MINIMO *alocar_vetor_caminho_minimo(int n_vertices)
{
    VETOR_CAMINHO_MINIMO *vetor = (VETOR_CAMINHO_MINIMO *)malloc(n_vertices * sizeof(VETOR_CAMINHO_MINIMO));
    verificar_alocacao(vetor, "falha ao alocar memória para o vetor de caminho minimo");
    inicializar_vetor_caminho_minimo(vetor, n_vertices);

    return vetor;
}

void imprimir_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO *vetor, int n_vertices)
{
    printf("Resultado (Vertice: Distancia, Anterior):\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("  Vertice %d: ", i + 1);
        if (vetor[i].distancia == INT_MAX)
        {
            printf("Dist: Infinito, Ant: %d\n", vetor[i].vertice_anterior);
        }
        else
            printf("Dist: %d, Ant: %d\n", vetor[i].distancia, vetor[i].vertice_anterior + 1);
    }
}

void imprimir_caminho(VETOR_CAMINHO_MINIMO *caminho, int vertice_inicial, int vertice_final)
{
    if (caminho != NULL && vertice_inicial > 0 && vertice_final > 0)
    {

        if (caminho[vertice_final - 1].distancia != INT_MAX)
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
 * IMPLEMENTAÇÃO: Dijkstra       *
 * *
 ****************************************/

VETOR_CAMINHO_MINIMO *dijkstra(GRAFO *grafo, int vertice_inicial)
{
    VETOR_CAMINHO_MINIMO *vetor_vertices = NULL;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_inicial > 0 && vertice_inicial <= grafo->n_vertices)
    {
        vetor_vertices = alocar_vetor_caminho_minimo(grafo->n_vertices);
        vetor_vertices[vertice_inicial - 1].distancia = 0;

        for (int qtdRelaxamentos = 0; qtdRelaxamentos < grafo->n_vertices - 1; qtdRelaxamentos++)
        {
            int distanciaMinima = INT_MAX, indiceVerticeAtual = 0;

            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if ((!vetor_vertices[i].vertice_visitado && vetor_vertices[i].distancia <= distanciaMinima))
                {
                    distanciaMinima = vetor_vertices[i].distancia;
                    indiceVerticeAtual = i;
                }
            }

            vetor_vertices[indiceVerticeAtual].vertice_visitado = 1;

            for (int i = 0; i < grafo->n_vertices; i++)
            {
                if (!vetor_vertices[i].vertice_visitado && grafo->matriz_adjacencia[indiceVerticeAtual][i].bolean)
                {
                    int dist = vetor_vertices[indiceVerticeAtual].distancia + grafo->matriz_adjacencia[indiceVerticeAtual][i].peso_aresta;
                    if (dist < vetor_vertices[i].distancia)
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

/****************************************
 * *
 * IMPLEMENTAÇÃO: Bellman-Ford     *
 * *
 ****************************************/

VETOR_CAMINHO_MINIMO *bellman_ford(GRAFO *grafo, int vertice_inicial, int *tem_ciclo_negativo)
{
    VETOR_CAMINHO_MINIMO *vetor_vertices = NULL;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_inicial > 0 && vertice_inicial <= grafo->n_vertices && tem_ciclo_negativo != NULL)
    {
        *tem_ciclo_negativo = 0; // Inicializa a flag de ciclo negativo

        if (!grafo->eh_digrafo && grafo->eh_ponderado)
        {
            printf("Aviso: Bellman-Ford esta sendo executado em um grafo nao direcionado e ponderado.\n");
        }

        // 1. Inicialização
        vetor_vertices = alocar_vetor_caminho_minimo(grafo->n_vertices);
        vetor_vertices[vertice_inicial - 1].distancia = 0;

        // 2. Relaxamento das arestas |V| - 1 vezes
        for (int i = 1; i < grafo->n_vertices; i++)
        {
            for (int u = 0; u < grafo->n_vertices; u++)
            {
                for (int v = 0; v < grafo->n_vertices; v++)
                {
                    if (grafo->matriz_adjacencia[u][v].bolean)
                    {
                        int peso = grafo->matriz_adjacencia[u][v].peso_aresta;
                        if (vetor_vertices[u].distancia != INT_MAX && vetor_vertices[u].distancia + peso < vetor_vertices[v].distancia)
                        {
                            vetor_vertices[v].distancia = vetor_vertices[u].distancia + peso;
                            vetor_vertices[v].vertice_anterior = u;
                        }
                    }
                }
            }
        }

        // 3. Verificação de ciclos de peso negativo
        for (int u = 0; u < grafo->n_vertices; u++)
        {
            for (int v = 0; v < grafo->n_vertices; v++)
            {
                if (grafo->matriz_adjacencia[u][v].bolean)
                {
                    int peso = grafo->matriz_adjacencia[u][v].peso_aresta;
                    if (vetor_vertices[u].distancia != INT_MAX && vetor_vertices[u].distancia + peso < vetor_vertices[v].distancia)
                    {
                        *tem_ciclo_negativo = 1;
                        liberar_vetor_caminho_minimo(&vetor_vertices);
                        return NULL;
                    }
                }
            }
        }
    }
    else
    {
        if (tem_ciclo_negativo != NULL)
            *tem_ciclo_negativo = 0; // Se os parâmetros não são válidos, define a flag como 0
    }

    return vetor_vertices;
}
