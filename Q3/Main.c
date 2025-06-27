#include <stdio.h>
#include <stdlib.h>
#include "Algoritmos/Algoritmos.h"

#define N_VERTICES 5 // Número de vértices do grafo
#define VERTICE_INICIAL 1 // Vértice inicial para os algoritmos de caminho máximo
#define VERTICE_FINAL 5   // Vértice final para os algoritmos de caminho máximo


int main()
{
    //Iniciar o grafo com N vertices, e ponderado
    GRAFO grafo = criar_grafo(1, 0, N_VERTICES); // Ponderado, Não Direcionado


    //Criando as arestas do grafo

    criar_aresta(&grafo, 1, 2, 0.1); 
    criar_aresta(&grafo, 1, 3, 0.5);
    criar_aresta(&grafo, 2, 4, 0.15);
    criar_aresta(&grafo, 3, 4, 0.2);
    criar_aresta(&grafo, 4, 5, 0.10);


    //Imprimindo a matriz de adjacência do grafo
    printf("Matriz de Adjacencia do Grafo:\n\n");
    imprimir_matriz_grafo(&grafo);

    //Calculando o caminho maximo
    printf("\nCalculando caminho maximo usando Dijkstra...\n");
    printf("Vertice Inicial: %d\n", VERTICE_INICIAL);
    VETOR_MAIOR_CAMINHO *caminho_dijkstra = dijkstra_maior_caminho(&grafo, VERTICE_INICIAL);

    if(caminho_dijkstra == NULL)
    {
        printf("Erro ao calcular o caminho maximo.\n");
    }

    //Imprimindo o vetor de caminho máximo encontrado por Dijkstra
    printf("\nVetor de caminho maximo encontrado por Dijkstra:\n");
    imprimir_vetor_maior_caminho(caminho_dijkstra, grafo.n_vertices); // Imprime o vetor de caminho máximo encontrado por Dijkstra

    imprimir_caminho(caminho_dijkstra, VERTICE_INICIAL, VERTICE_FINAL); // Imprime o caminho do vértice inicial ao final

    //Liberando a memória alocada para o vetor de caminho máximo
    liberar_vetor_maior_caminho(&caminho_dijkstra);

    //Liberando a memória alocada para o grafo
    liberar_grafo(&grafo);

    return 0;
}