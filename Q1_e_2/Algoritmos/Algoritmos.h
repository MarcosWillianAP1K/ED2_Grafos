#include "../Grafos/Grafos.h"

#pragma once

typedef struct VETOR_CAMINHO_MINIMO
{
    int vertice_anterior;
    short int vertice_visitado; // Usado apenas por Dijkstra
    int distancia;
} VETOR_CAMINHO_MINIMO;

void liberar_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO **vetor);

void inicializar_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO *vetor, int n_vertices);


VETOR_CAMINHO_MINIMO *alocar_vetor_caminho_minimo(int n_vertices);

void imprimir_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO *vetor, int n_vertices);

void imprimir_caminho(VETOR_CAMINHO_MINIMO *caminho, int vertice_inicial, int vertice_final);

VETOR_CAMINHO_MINIMO *dijkstra(GRAFO *grafo, int vertice_inicial);

VETOR_CAMINHO_MINIMO *bellman_ford(GRAFO *grafo, int vertice_inicial, int *tem_ciclo_negativo);