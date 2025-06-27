#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX

/****************************************
 * *
 * IMPLEMENTAÇÃO: Utilitarios      *
 * *
 ****************************************/


void verificar_alocacao(void *ptr, const char *mensagem)
{
    if (!ptr)
    {
        fprintf(stderr, "Erro de alocacao: %s\n", mensagem);
        exit(EXIT_FAILURE);
    }
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: matriz_adjacencia  *
 * *
 ****************************************/

// Estruturas (originalmente em matriz_adjacencia.h)
typedef struct MATRIZ_ADJACENCIA
{
    short int bolean;
    int peso_aresta; // Peso da aresta, se for ponderada

} MATRIZ_ADJACENCIA;

typedef struct VERTICE
{
    int grau; // Grau do vértice
    int peso; // Peso do vértice, se for ponderado
} VERTICE;

typedef struct GRAFO
{
    short int eh_ponderado;                // 1 se o grafo é ponderado, 0 se não
    short int eh_digrafo;                  // 1 se o grafo é um dígrafo, 0 se não
    int n_vertices;                        // Número de vértices do grafo
    MATRIZ_ADJACENCIA **matriz_adjacencia; // Matriz de adjacência
    VERTICE *vetor_vertices;               // Vetor de vértices
} GRAFO;


GRAFO iniciar_grafo(short int eh_ponderado, short int eh_digrafo)
{
    GRAFO grafo;

    grafo.eh_ponderado = eh_ponderado;
    grafo.eh_digrafo = eh_digrafo;
    grafo.n_vertices = 0;
    grafo.matriz_adjacencia = NULL;
    grafo.vetor_vertices = NULL;

    return grafo;
}

GRAFO criar_grafo(short int eh_ponderado, short int eh_digrafo, int n_vertices)
{
    GRAFO grafo = iniciar_grafo(eh_ponderado, eh_digrafo);

    grafo.n_vertices = n_vertices;

    grafo.vetor_vertices = (VERTICE *)malloc(n_vertices * sizeof(VERTICE));
    verificar_alocacao(grafo.vetor_vertices, "falha ao alocar memória para o vetor de vértices");

    for (int i = 0; i < n_vertices; i++)
    {
        grafo.vetor_vertices[i].grau = 0;
        grafo.vetor_vertices[i].peso = 0;
    }

    grafo.matriz_adjacencia = (MATRIZ_ADJACENCIA **)malloc(n_vertices * sizeof(MATRIZ_ADJACENCIA *));
    verificar_alocacao(grafo.matriz_adjacencia, "falha ao alocar memória para a matriz de adjacência");

    for (int i = 0; i < n_vertices; i++)
    {
        grafo.matriz_adjacencia[i] = (MATRIZ_ADJACENCIA *)malloc(n_vertices * sizeof(MATRIZ_ADJACENCIA));
        verificar_alocacao(grafo.matriz_adjacencia[i], "falha ao alocar memória para a linha da matriz de adjacência");

        for (int j = 0; j < n_vertices; j++)
        {
            grafo.matriz_adjacencia[i][j].bolean = 0;
            grafo.matriz_adjacencia[i][j].peso_aresta = 0;
        }
    }

    return grafo;
}

void liberar_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {
        if (grafo->matriz_adjacencia != NULL)
        {
            for (int i = 0; i < grafo->n_vertices; i++)
            {
                free(grafo->matriz_adjacencia[i]);
            }
            free(grafo->matriz_adjacencia);
        }
        grafo->matriz_adjacencia = NULL;

        if (grafo->vetor_vertices != NULL)
        {
            free(grafo->vetor_vertices);
        }
        grafo->vetor_vertices = NULL;

        grafo->n_vertices = 0;
    }
}

short int criar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino, int peso)
{
    short int retorno = 0;

    if (grafo != NULL && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices && ((!grafo->eh_ponderado && peso == 1) || (grafo->eh_ponderado)))
    {
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].bolean = 1;
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].peso_aresta = peso;
        grafo->vetor_vertices[vertice_origem - 1].grau++;

        if (!grafo->eh_digrafo)
        {
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].bolean = 1;
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].peso_aresta = peso;
            grafo->vetor_vertices[vertice_destino - 1].grau++;
        }

        retorno = 1;
    }

    return retorno;
}

short int apagar_aresta(GRAFO *grafo, int vertice_origem, int vertice_destino)
{
    short int retorno = 0;

    if (grafo != NULL && grafo->n_vertices > 0 && vertice_origem > 0 && vertice_origem <= grafo->n_vertices && vertice_destino > 0 && vertice_destino <= grafo->n_vertices)
    {

        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].bolean = 0;
        grafo->matriz_adjacencia[vertice_origem - 1][vertice_destino - 1].peso_aresta = 0;
        grafo->vetor_vertices[vertice_origem - 1].grau--;

        if (!grafo->eh_digrafo)
        {
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].bolean = 0;
            grafo->matriz_adjacencia[vertice_destino - 1][vertice_origem - 1].peso_aresta = 0;
            grafo->vetor_vertices[vertice_destino - 1].grau--;
        }

        retorno = 1;
    }

    return retorno;
}

void imprimir_matriz_grafo(GRAFO *grafo)
{
    if (grafo != NULL)
    {
        printf("Matriz de Adjacencia:\n");
        for (int i = -2; i < grafo->n_vertices; i++)
        {

            if (i < 0)
            {
                printf("   ");
                for (int j = 0; j < grafo->n_vertices; j++)
                {
                    if (i == -1)
                    {
                        printf("__");
                    }
                    else
                    {
                        printf("%d ", j + 1);
                    }
                }
            }
            else
            {
                printf("%d| ", i + 1);
                for (int j = 0; j < grafo->n_vertices; j++)
                {
                    if (i == -1)
                    {
                        printf("%d ", j + 1);
                    }
                    else
                    {
                        printf("%d ", grafo->matriz_adjacencia[i][j].bolean);
                    }
                }
            }

            printf("\n");
        }
    }
    else
    {
        printf("Grafo não inicializado.\n");
    }
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: Vetor caminho minimo       *
 * *
 ****************************************/

// Estrutura. Reutilizada por Dijkstra e Bellman-Ford.
typedef struct VETOR_CAMINHO_MINIMO
{
    int vertice_anterior;
    short int vertice_visitado; // Usado apenas por Dijkstra
    int distancia;
} VETOR_CAMINHO_MINIMO;

// Definições das funções
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
    printf("Resultado (Vértice: Distância, Anterior):\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("  Vértice %d: ", i + 1);
        if (vetor[i].distancia == INT_MAX)
        {
            printf("Dist: Infinito, Ant: %d\n", vetor[i].vertice_anterior);
        }
        else
            printf("Dist: %d, Ant: %d\n", vetor[i].distancia, vetor[i].vertice_anterior + 1);
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
    }

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

    return vetor_vertices;
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: Bellman-Ford     *
 * *
 ****************************************/

VETOR_CAMINHO_MINIMO *bellman_ford(GRAFO *grafo, int vertice_inicial, int *tem_ciclo_negativo)
{
    if (grafo == NULL || grafo->n_vertices <= 0 || vertice_inicial <= 0 || vertice_inicial > grafo->n_vertices || tem_ciclo_negativo == NULL)
    {
        if (tem_ciclo_negativo != NULL)
            *tem_ciclo_negativo = 0;
        return NULL;
    }

    if (!grafo->eh_digrafo && grafo->eh_ponderado)
    {
        printf("Aviso: Bellman-Ford esta sendo executado em um grafo nao direcionado e ponderado.\n");
    }

    *tem_ciclo_negativo = 0;

    // 1. Inicialização
    VETOR_CAMINHO_MINIMO *vetor_vertices = alocar_vetor_caminho_minimo(grafo->n_vertices);
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

    return vetor_vertices;
}


//=========================FUNÇÃO PRINCIPAL (MAIN)=========================
int main()
{
    //=========================EXEMPLO DE USO DIJKSTRA=========================
    printf("--- Testando Dijkstra ---\n");
    GRAFO grafo_dijkstra = criar_grafo(0, 0, 5); // Ponderado, Não-direcionado, 5 vértices

    criar_aresta(&grafo_dijkstra, 1, 2, 1);
    criar_aresta(&grafo_dijkstra, 1, 5, 1);
    criar_aresta(&grafo_dijkstra, 2, 3, 1);
    criar_aresta(&grafo_dijkstra, 3, 4, 1);
    criar_aresta(&grafo_dijkstra, 4, 5, 1);

    imprimir_matriz_grafo(&grafo_dijkstra);

    int vertice_inicial_dijkstra = 1;
    VETOR_CAMINHO_MINIMO *resultado_dijkstra = dijkstra(&grafo_dijkstra, vertice_inicial_dijkstra);

    if (resultado_dijkstra != NULL)
    {
        printf("\nResultado Dijkstra (partindo do vertice %d):\n", vertice_inicial_dijkstra);
        imprimir_vetor_caminho_minimo(resultado_dijkstra, grafo_dijkstra.n_vertices);
        liberar_vetor_caminho_minimo(&resultado_dijkstra);
    }
    liberar_grafo(&grafo_dijkstra);

    //=========================EXEMPLO DE USO BELLMAN-FORD=========================
    printf("\n\n--- Testando Bellman-Ford ---\n");

    // Teste 1: Grafo direcionado sem ciclos negativos
    printf("\n--- Teste 1: Grafo direcionado com aresta negativa (sem ciclo) ---\n");
    GRAFO grafo_bf_1 = criar_grafo(0, 0, 5); // Ponderado, Direcionado, 5 vértices

    criar_aresta(&grafo_bf_1, 1, 2, 1);
    criar_aresta(&grafo_bf_1, 1, 5, 1);
    criar_aresta(&grafo_bf_1, 2, 3, 1);
    criar_aresta(&grafo_bf_1, 3, 4, 1);
    criar_aresta(&grafo_bf_1, 4, 5, 1);

    imprimir_matriz_grafo(&grafo_bf_1);

    int tem_ciclo_negativo = 0;
    int vertice_inicial_bf = 1;
    VETOR_CAMINHO_MINIMO *resultado_bf = bellman_ford(&grafo_bf_1, vertice_inicial_bf, &tem_ciclo_negativo);

    if (tem_ciclo_negativo)
    {
        printf("\nResultado Bellman-Ford: Ciclo de peso negativo detectado.\n");
    }
    else if (resultado_bf != NULL)
    {
        printf("\nResultado Bellman-Ford (partindo do vertice %d):\n", vertice_inicial_bf);
        imprimir_vetor_caminho_minimo(resultado_bf, grafo_bf_1.n_vertices);
        liberar_vetor_caminho_minimo(&resultado_bf);
    }
    liberar_grafo(&grafo_bf_1);

    // Teste 2: Grafo com ciclo negativo
    printf("\n\n--- Teste 2: Grafo com ciclo de peso negativo ---\n");
    GRAFO grafo_bf_2 = criar_grafo(1, 1, 3); // Ponderado, Direcionado, 3 vértices
    criar_aresta(&grafo_bf_2, 1, 2, 1);
    criar_aresta(&grafo_bf_2, 2, 3, 2);
    criar_aresta(&grafo_bf_2, 3, 2, -4); // Cria um ciclo 2 -> 3 -> 2 com peso 2 + (-4) = -2

    imprimir_matriz_grafo(&grafo_bf_2);

    tem_ciclo_negativo = 0; // Reseta a flag
    resultado_bf = bellman_ford(&grafo_bf_2, vertice_inicial_bf, &tem_ciclo_negativo);

    if (tem_ciclo_negativo)
    {
        printf("\nResultado Bellman-Ford: Ciclo de peso negativo detectado. (CORRETO)\n");
    }
    else if (resultado_bf != NULL)
    {
        printf("\nResultado Bellman-Ford encontrado, o que eh inesperado.\n");
        liberar_vetor_caminho_minimo(&resultado_bf);
    }
    else
    {
        printf("\nNao foi possivel executar Bellman-Ford, provavelmente devido ao ciclo. (CORRETO)\n");
    }

    liberar_grafo(&grafo_bf_2);

    return 0;
}