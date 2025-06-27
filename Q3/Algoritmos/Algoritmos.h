#include "../Grafos/Grafos.h"

#pragma once


typedef struct VETOR_MAIOR_CAMINHO
{
    int vertice_anterior;
    short int vertice_visitado; 
    float distancia;
} VETOR_MAIOR_CAMINHO;


void liberar_vetor_maior_caminho(VETOR_MAIOR_CAMINHO **vetor);

void inicializar_vetor_maior_caminho(VETOR_MAIOR_CAMINHO *vetor, int n_vertices);

VETOR_MAIOR_CAMINHO *alocar_vetor_maior_caminho(int n_vertices);

void imprimir_vetor_maior_caminho(VETOR_MAIOR_CAMINHO *vetor, int n_vertices);

void imprimir_caminho(VETOR_MAIOR_CAMINHO *caminho, int vertice_inicial, int vertice_final);

VETOR_MAIOR_CAMINHO *dijkstra_maior_caminho(GRAFO *grafo, int vertice_inicial);
