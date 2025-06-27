#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX
#include <string.h> // Para memset (embora eu tenha trocado por loops)
#include <math.h>   // Para pow

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
    verificar_alocacao(grafo.vetor_vertices, "falha ao alocar memoria para o vetor de vertices");

    for (int i = 0; i < n_vertices; i++)
    {
        grafo.vetor_vertices[i].grau = 0;
        grafo.vetor_vertices[i].peso = 0;
    }

    grafo.matriz_adjacencia = (MATRIZ_ADJACENCIA **)malloc(n_vertices * sizeof(MATRIZ_ADJACENCIA *));
    verificar_alocacao(grafo.matriz_adjacencia, "falha ao alocar memoria para a matriz de adjacencia");

    for (int i = 0; i < n_vertices; i++)
    {
        grafo.matriz_adjacencia[i] = (MATRIZ_ADJACENCIA *)malloc(n_vertices * sizeof(MATRIZ_ADJACENCIA));
        verificar_alocacao(grafo.matriz_adjacencia[i], "falha ao alocar memoria para a linha da matriz de adjacencia");

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
        printf("Matriz de Adjacencia (1 = aresta, 0 = sem aresta):\n");
        printf("   ");
        for (int j = 0; j < grafo->n_vertices; j++)
        {
            printf("%3d", j + 1); // Cabeçalho das colunas (base 1)
        }
        printf("\n");
        printf("   ");
        for (int j = 0; j < grafo->n_vertices; j++)
        {
            printf("---");
        }
        printf("\n");

        for (int i = 0; i < grafo->n_vertices; i++)
        {
            printf("%d |", i + 1); // Cabeçalho das linhas (base 1)
            for (int j = 0; j < grafo->n_vertices; j++)
            {
                printf("%3d", grafo->matriz_adjacencia[i][j].bolean);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Grafo nao inicializado.\n");
    }
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: Vetor caminho minimo       *
 * *
 ****************************************/

typedef struct VETOR_CAMINHO_MINIMO
{
    int vertice_anterior;
    short int vertice_visitado; // Usado apenas por Dijkstra
    int distancia;
} VETOR_CAMINHO_MINIMO;

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
    verificar_alocacao(vetor, "falha ao alocar memoria para o vetor de caminho minimo");
    inicializar_vetor_caminho_minimo(vetor, n_vertices);
    return vetor;
}

void imprimir_vetor_caminho_minimo(VETOR_CAMINHO_MINIMO *vetor, int n_vertices)
{
    printf("Resultado (Vertice: Distancia, Anterior):\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("  Vertice %d: ", i + 1); // Exibe base 1
        if (vetor[i].distancia == INT_MAX)
        {
            printf("Dist: Infinito, Ant: %d\n", vetor[i].vertice_anterior == -1 ? -1 : vetor[i].vertice_anterior + 1); // Exibe base 1
        }
        else
            printf("Dist: %d, Ant: %d\n", vetor[i].distancia, vetor[i].vertice_anterior == -1 ? -1 : vetor[i].vertice_anterior + 1); // Exibe base 1
    }
}

// Nova função para imprimir a sequência do caminho mínimo
// Recebe array de predecessores (p), e índices de início e fim (base 0)
void caminho_minimo_imprimir_sequencia(int *p, int s_idx, int f_idx, int n_vertices_grafo)
{
    // Definindo um tamanho máximo para o caminho. Usa n_vertices_grafo para ser mais genérico.
    int caminho[n_vertices_grafo];
    int pos = 0;
    int current = f_idx;

    printf("Caminho minimo entre vertices %d e %d:\n", s_idx + 1, f_idx + 1);

    // Verificar se o destino é alcançável (além do próprio início se for o caso)
    if (p[f_idx] == -1 && s_idx != f_idx)
    {
        printf("Nao ha caminho do vertice %d para o vertice %d.\n", s_idx + 1, f_idx + 1);
        return;
    }

    // Reconstruir o caminho
    while (current != -1 && pos < n_vertices_grafo)
    {
        caminho[pos++] = current;
        if (current == s_idx)
            break; // Parar quando o vértice inicial for encontrado
        current = p[current];
    }

    // Imprimir o caminho na ordem correta
    printf("Sequencia: ");
    for (int i = pos - 1; i >= 0; i--)
    {
        printf("%d ", caminho[i] + 1); // Exibe base 1
    }
    printf("\n");
}

/****************************************
 * *
 * IMPLEMENTAÇÃO: Dijkstra       *
 * *
 ****************************************/

// vertice_inicial_idx é agora um índice base 0
VETOR_CAMINHO_MINIMO *dijkstra(GRAFO *grafo, int vertice_inicial_idx)
{
    VETOR_CAMINHO_MINIMO *vetor_vertices = NULL;

    if (grafo == NULL || grafo->n_vertices <= 0 || vertice_inicial_idx < 0 || vertice_inicial_idx >= grafo->n_vertices)
    {
        return NULL;
    }

    vetor_vertices = alocar_vetor_caminho_minimo(grafo->n_vertices);
    vetor_vertices[vertice_inicial_idx].distancia = 0;

    for (int count = 0; count < grafo->n_vertices - 1; count++) // Loop para relaxar V-1 vezes
    {
        int distanciaMinima = INT_MAX;
        int indiceVerticeAtual = -1;

        // Encontra o vértice não visitado com a menor distância
        for (int i = 0; i < grafo->n_vertices; i++)
        {
            if (!vetor_vertices[i].vertice_visitado && vetor_vertices[i].distancia < distanciaMinima)
            {
                distanciaMinima = vetor_vertices[i].distancia;
                indiceVerticeAtual = i;
            }
        }

        if (indiceVerticeAtual == -1 || distanciaMinima == INT_MAX)
        { // Todos os vértices alcançáveis foram visitados ou o resto é inacessível
            break;
        }

        vetor_vertices[indiceVerticeAtual].vertice_visitado = 1;

        // Atualiza as distâncias dos vértices adjacentes
        for (int v = 0; v < grafo->n_vertices; v++)
        {
            // Verifica se existe aresta e se o vértice de destino ainda não foi visitado
            if (grafo->matriz_adjacencia[indiceVerticeAtual][v].bolean && !vetor_vertices[v].vertice_visitado)
            {
                int peso_aresta = grafo->matriz_adjacencia[indiceVerticeAtual][v].peso_aresta;
                // Relaxamento
                if (vetor_vertices[indiceVerticeAtual].distancia != INT_MAX &&
                    vetor_vertices[indiceVerticeAtual].distancia + peso_aresta < vetor_vertices[v].distancia)
                {
                    vetor_vertices[v].distancia = vetor_vertices[indiceVerticeAtual].distancia + peso_aresta;
                    vetor_vertices[v].vertice_anterior = indiceVerticeAtual; // CORRIGIDO: predecessor é indiceVerticeAtual
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

// vertice_inicial_idx é agora um índice base 0
VETOR_CAMINHO_MINIMO *bellman_ford(GRAFO *grafo, int vertice_inicial_idx, int *tem_ciclo_negativo)
{
    if (grafo == NULL || grafo->n_vertices <= 0 || vertice_inicial_idx < 0 || vertice_inicial_idx >= grafo->n_vertices || tem_ciclo_negativo == NULL)
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
    vetor_vertices[vertice_inicial_idx].distancia = 0;

    // 2. Relaxamento das arestas |V| - 1 vezes
    for (int i = 1; i < grafo->n_vertices; i++)
    {
        short int houve_atualizacao = 0; // Otimização: Se não houver atualização, podemos parar mais cedo
        for (int u = 0; u < grafo->n_vertices; u++)
        {
            if (vetor_vertices[u].distancia == INT_MAX)
                continue; // Otimização: Não relaxar a partir de vértices inalcançáveis ainda
            for (int v = 0; v < grafo->n_vertices; v++)
            {
                if (grafo->matriz_adjacencia[u][v].bolean)
                {
                    int peso = grafo->matriz_adjacencia[u][v].peso_aresta;
                    if (vetor_vertices[u].distancia + peso < vetor_vertices[v].distancia)
                    {
                        vetor_vertices[v].distancia = vetor_vertices[u].distancia + peso;
                        vetor_vertices[v].vertice_anterior = u;
                        houve_atualizacao = 1; // Houve uma atualização
                    }
                }
            }
        }
        if (!houve_atualizacao)
        { // Se nenhuma distância foi atualizada nesta iteração, terminamos
            break;
        }
    }

    // 3. Verificação de ciclos de peso negativo
    for (int u = 0; u < grafo->n_vertices; u++)
    {
        if (vetor_vertices[u].distancia == INT_MAX)
            continue; // Otimização
        for (int v = 0; v < grafo->n_vertices; v++)
        {
            if (grafo->matriz_adjacencia[u][v].bolean)
            {
                int peso = grafo->matriz_adjacencia[u][v].peso_aresta;
                if (vetor_vertices[u].distancia + peso < vetor_vertices[v].distancia)
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

/****************************************
 * *
 * IMPLEMENTAÇÃO: Torre de Hanoi     *
 * *
 ****************************************/

#define N_HANOI 3                                // Número de discos (pode ser alterado)
#define PINOS_HANOI 3                            // Número de pinos (geralmente 3)
#define MAX_HANOI 27 // Total de configurações (Pinos ^ N)

// Estrutura para representar uma configuração da Torre de Hanói
typedef struct
{
    int conf[N_HANOI]; // conf[i] armazena o pino do disco i (0 a N_HANOI-1), onde disco 0 é o menor.
                       // Pinos são 1, 2, 3
} HANOI;

// Funções auxiliares para a lógica da Torre de Hanói
// Retorna o menor disco no topo de um pino (índice do disco), ou N_HANOI se o pino estiver vazio
int disco_no_topo(const HANOI v, int pino_idx)
{
    int topo_disco = N_HANOI; // N_HANOI significa que nenhum disco está no pino (maior que qualquer disco válido 0..N_HANOI-1)
    for (int i = 0; i < N_HANOI; ++i)
    {
        if (v.conf[i] == pino_idx)
        { // Se o disco 'i' está no pino 'pino_idx'
            if (topo_disco == N_HANOI || i < topo_disco)
            { // Se for o primeiro disco encontrado ou um menor
                topo_disco = i;
            }
        }
    }
    return topo_disco;
}

// Verifica se uma configuração é válida (disco menor sempre sobre disco maior)
short int is_valid_hanoi_config(const HANOI v)
{
    for (int pino_idx = 1; pino_idx <= PINOS_HANOI; ++pino_idx)
    {
        int maior_disco_na_base = -1; // representa o disco mais baixo/maior
        
        for (int i = 0; i < N_HANOI; ++i)
        {
            if (v.conf[i] == pino_idx)
            {
                // Se encontramos um disco 'i' no pino 'pino_idx'
                // e já temos um disco na base e o disco atual 'i' é maior que a base
                if (maior_disco_na_base != -1 && i > maior_disco_na_base)
                {
                    return 0; // Configuração inválida
                }
                maior_disco_na_base = i; // Atualiza o disco mais baixo (maior disco encontrado até agora)
            }
        }
    }
    return 1; // Configuração válida
}

// Gera todas as Pinos^N combinações possíveis e armazena em um array de HANOI
void gerar_configuracoes_hanoi(HANOI *configs)
{
    for (int i = 0; i < MAX_HANOI; ++i)
    {
        int num = i;
        for (int j = 0; j < N_HANOI; ++j)
        {
            configs[i].conf[j] = (num % PINOS_HANOI) + 1; // Mapeia 0,1,2 para 1,2,3 (pinos)
            num /= PINOS_HANOI;
        }
    }
}

// Verifica se um movimento entre v1 e v2 é válido de acordo com as regras da Torre de Hanói
short int aresta_hanoi(const HANOI v1, const HANOI v2)
{
    int diff_count = 0;
    int disco_movido_idx = -1; // Índice do disco que mudou de pino
    int pino_origem = -1;
    int pino_destino = -1;

    // 1. Verificar se apenas um disco se moveu
    for (int i = 0; i < N_HANOI; ++i)
    {
        if (v1.conf[i] != v2.conf[i])
        {
            diff_count++;
            disco_movido_idx = i;
            pino_origem = v1.conf[i];
            pino_destino = v2.conf[i];
        }
    }

    if (diff_count != 1)
    {
        return 0; // Mais de um disco se moveu ou nenhum disco se moveu
    }

    // 2. Verificar se a configuração inicial e final são válidas
    if (!is_valid_hanoi_config(v1) || !is_valid_hanoi_config(v2))
    {
        return 0; // Uma das configurações (origem ou destino) é inválida pelas regras
    }

    // 3. Verificar se o disco movido é o disco do topo do pino de origem
    if (disco_movido_idx != disco_no_topo(v1, pino_origem))
    {
        return 0; // O disco movido não era o disco do topo do pino de origem
    }

    // 4. Verificar se o disco movido é colocado sobre um disco maior (ou pino vazio) no pino de destino
    for (int i = 0; i < N_HANOI; ++i)
    {
        if (i != disco_movido_idx && v2.conf[i] == pino_destino)
        {
            // Se outro disco 'i' está no pino de destino de v2
            if (i < disco_movido_idx)
            {             // Se esse outro disco 'i' (menor índice = menor disco) é MENOR que o disco que foi movido
                return 0; // Movimento inválido: disco menor sobre disco maior
            }
        }
    }
    return 1; // Movimento válido
}

// Constrói o grafo (GRAFO) a partir das configurações de Hanói
void construir_grafo_hanoi(GRAFO *grafo, const HANOI *configs)
{
    // Reconfigura o grafo existente para o tamanho da Torre de Hanói
    if (grafo->n_vertices != 0)
    { // Se o grafo já foi criado, liberamos para recriar
        liberar_grafo(grafo);
    }
    *grafo = criar_grafo(1, 1, MAX_HANOI); // Grafo da Torre de Hanói é ponderado (peso 1 por movimento) e direcionado

    for (int i = 0; i < MAX_HANOI; ++i)
    {
        for (int j = 0; j < MAX_HANOI; ++j)
        {
            // Se o movimento é válido, adiciona uma aresta com peso 1
            if (aresta_hanoi(configs[i], configs[j]))
            {
                grafo->matriz_adjacencia[i][j].bolean = 1;
                grafo->matriz_adjacencia[i][j].peso_aresta = 1;
                grafo->vetor_vertices[i].grau++; // Apenas incrementa o grau do vertice de origem para digrafos
            }
            else
            {
                grafo->matriz_adjacencia[i][j].bolean = 0;
                grafo->matriz_adjacencia[i][j].peso_aresta = 0;
            }
        }
    }
}

// Função para imprimir uma configuração da Torre de Hanói
void imprimir_configuracao_hanoi(const HANOI *configs, int idx)
{
    printf("Vertice %d: Discos nos pinos [", idx);
    for (int j = 0; j < N_HANOI; ++j)
    {
        printf("%d", configs[idx].conf[j]);
        if (j < N_HANOI - 1)
            printf(", ");
    }
    printf("]");
    if (!is_valid_hanoi_config(configs[idx]))
    {
        printf(" (INVALIDA para Hanoí)");
    }
    printf("\n");
}

//=========================FUNÇÃO PRINCIPAL (MAIN)=========================
int main()
{
    int opcao_menu_principal;

    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Testar algoritmos de grafo genericos\n");
        printf("2 - Resolver Torre de Hanoi\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao_menu_principal);

        switch (opcao_menu_principal)
        {
        case 1:
        { // Testar algoritmos de grafo genericos
            printf("\n--- Testando Dijkstra (Grafo Generico) ---\n");
            GRAFO grafo_dijkstra = criar_grafo(0, 0, 5); // Não ponderado, Não-direcionado, 5 vértices

            criar_aresta(&grafo_dijkstra, 1, 2, 1);
            criar_aresta(&grafo_dijkstra, 1, 5, 1);
            criar_aresta(&grafo_dijkstra, 2, 3, 1);
            criar_aresta(&grafo_dijkstra, 3, 4, 1);
            criar_aresta(&grafo_dijkstra, 4, 5, 1);

            imprimir_matriz_grafo(&grafo_dijkstra);

            int vertice_inicial_dijkstra_main = 1;                                                                   // Vertice base 1
            VETOR_CAMINHO_MINIMO *resultado_dijkstra = dijkstra(&grafo_dijkstra, vertice_inicial_dijkstra_main - 1); // Passa indice base 0

            if (resultado_dijkstra != NULL)
            {
                printf("\nResultado Dijkstra (partindo do vertice %d):\n", vertice_inicial_dijkstra_main);
                imprimir_vetor_caminho_minimo(resultado_dijkstra, grafo_dijkstra.n_vertices);
                liberar_vetor_caminho_minimo(&resultado_dijkstra);
            }
            liberar_grafo(&grafo_dijkstra);

            printf("\n\n--- Testando Bellman-Ford (Grafo Generico) ---\n");

            printf("\n--- Teste 1: Grafo direcionado com aresta negativa (sem ciclo) ---\n");
            GRAFO grafo_bf_1 = criar_grafo(1, 1, 5); // Ponderado, Direcionado, 5 vértices

            // Exemplo com arestas e pesos para Bellman-Ford
            criar_aresta(&grafo_bf_1, 1, 2, 6);
            criar_aresta(&grafo_bf_1, 1, 4, 7);
            criar_aresta(&grafo_bf_1, 2, 3, 5);
            criar_aresta(&grafo_bf_1, 2, 5, -4);
            criar_aresta(&grafo_bf_1, 3, 2, -2);
            criar_aresta(&grafo_bf_1, 4, 3, -3);
            criar_aresta(&grafo_bf_1, 4, 5, 9);
            criar_aresta(&grafo_bf_1, 5, 1, 2);
            criar_aresta(&grafo_bf_1, 5, 3, 7);

            imprimir_matriz_grafo(&grafo_bf_1);

            int tem_ciclo_negativo = 0;
            int vertice_inicial_bf_main = 1;                                                                                  // Vertice base 1
            VETOR_CAMINHO_MINIMO *resultado_bf = bellman_ford(&grafo_bf_1, vertice_inicial_bf_main - 1, &tem_ciclo_negativo); // Passa indice base 0

            if (tem_ciclo_negativo)
            {
                printf("\nResultado Bellman-Ford: Ciclo de peso negativo detectado.\n");
            }
            else if (resultado_bf != NULL)
            {
                printf("\nResultado Bellman-Ford (partindo do vertice %d):\n", vertice_inicial_bf_main);
                imprimir_vetor_caminho_minimo(resultado_bf, grafo_bf_1.n_vertices);
                liberar_vetor_caminho_minimo(&resultado_bf);
            }
            liberar_grafo(&grafo_bf_1);

            printf("\n\n--- Teste 2: Grafo com ciclo de peso negativo ---\n");
            GRAFO grafo_bf_2 = criar_grafo(1, 1, 3); // Ponderado, Direcionado, 3 vértices
            criar_aresta(&grafo_bf_2, 1, 2, 1);
            criar_aresta(&grafo_bf_2, 2, 3, 2);
            criar_aresta(&grafo_bf_2, 3, 2, -4); // Cria um ciclo 2 -> 3 -> 2 com peso 2 + (-4) = -2

            imprimir_matriz_grafo(&grafo_bf_2);

            tem_ciclo_negativo = 0;                                           // Reseta a flag
            resultado_bf = bellman_ford(&grafo_bf_2, 0, &tem_ciclo_negativo); // Testando com vertice 0 (vertice 1)

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
            break;
        }
        case 2:
        { // Resolver Torre de Hanoi
            printf("\n--- Resolvendo Torre de Hanoi ---\n");
            printf("Parametros: N = %d discos, Pinos = %d\n", N_HANOI, PINOS_HANOI);
            printf("Total de configuracoes/vertices: %d\n", MAX_HANOI);

            HANOI configs_hanoi[MAX_HANOI];
            GRAFO grafo_hanoi; // O grafo real que vai ser preenchido

            gerar_configuracoes_hanoi(configs_hanoi);
            construir_grafo_hanoi(&grafo_hanoi, configs_hanoi);

            int sub_opcao;
            do
            {
                printf("\n===== MENU TORRE DE HANOI =====\n");
                printf("1 - Exibir todas as configuracoes (vertices)\n");
                printf("2 - Exibir matriz de adjacencia da Torre de Hanoi\n");
                printf("3 - Encontrar caminho minimo (Dijkstra) na Torre de Hanoi\n");
                printf("4 - Encontrar caminho minimo (Bellman-Ford) na Torre de Hanoi\n");
                printf("0 - Voltar ao Menu Principal\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &sub_opcao);

                switch (sub_opcao)
                {
                case 1:
                {
                    printf("\nConfiguracoes dos vertices da Torre de Hanoi:\n");
                    for (int i = 0; i < MAX_HANOI; ++i)
                    {
                        imprimir_configuracao_hanoi(configs_hanoi, i);
                    }
                    break;
                }
                case 2:
                {
                    printf("\nMatriz de Adjacencia da Torre de Hanoi:\n");
                    // Reutiliza a funcao de imprimir matriz de grafo generica
                    // mas ajusta a exibicao para representar arestas como 0 ou 1
                    printf("   ");
                    for (int j = 0; j < grafo_hanoi.n_vertices; j++)
                    {
                        printf("%3d", j); // Exibe indices base 0 para configs_hanoi
                    }
                    printf("\n");
                    printf("   ");
                    for (int j = 0; j < grafo_hanoi.n_vertices; j++)
                    {
                        printf("---");
                    }
                    printf("\n");
                    for (int i = 0; i < grafo_hanoi.n_vertices; i++)
                    {
                        printf("%d |", i);
                        for (int j = 0; j < grafo_hanoi.n_vertices; j++)
                        {
                            printf("%3d", grafo_hanoi.matriz_adjacencia[i][j].bolean);
                        }
                        printf("\n");
                    }
                    break;
                }
                case 3:
                { // Dijkstra para Torre de Hanoi
                    int inicio_hanoi, fim_hanoi;
                    printf("Digite o indice do vertice inicial (0 a %d): ", MAX_HANOI - 1);
                    scanf("%d", &inicio_hanoi);
                    printf("Digite o indice do vertice final (0 a %d): ", MAX_HANOI - 1);
                    scanf("%d", &fim_hanoi);

                    if (inicio_hanoi < 0 || inicio_hanoi >= MAX_HANOI || fim_hanoi < 0 || fim_hanoi >= MAX_HANOI)
                    {
                        printf("Indices de vertice invalidos.\n");
                        break;
                    }
                    if (!is_valid_hanoi_config(configs_hanoi[inicio_hanoi]))
                    {
                        printf("Configuracao inicial (vertice %d) nao e valida para Torre de Hanoi.\n", inicio_hanoi);
                        break;
                    }
                    if (!is_valid_hanoi_config(configs_hanoi[fim_hanoi]))
                    {
                        printf("Configuracao final (vertice %d) nao e valida para Torre de Hanoi.\n", fim_hanoi);
                        break;
                    }

                    VETOR_CAMINHO_MINIMO *res_dijkstra_hanoi = dijkstra(&grafo_hanoi, inicio_hanoi);
                    if (res_dijkstra_hanoi != NULL)
                    {
                        printf("\nCaminho Minimo com Dijkstra (Torre de Hanoi):\n");
                        // Extrai o array de predecessores para passar para a função de impressão da sequência
                        int *predecessores_dijkstra = (int *)malloc(grafo_hanoi.n_vertices * sizeof(int));
                        verificar_alocacao(predecessores_dijkstra, "falha ao alocar predecessores para imprimir");
                        for (int i = 0; i < grafo_hanoi.n_vertices; ++i)
                        {
                            predecessores_dijkstra[i] = res_dijkstra_hanoi[i].vertice_anterior;
                        }
                        caminho_minimo_imprimir_sequencia(predecessores_dijkstra, inicio_hanoi, fim_hanoi, grafo_hanoi.n_vertices);
                        free(predecessores_dijkstra);

                        printf("Distancia minima: %d movimentos\n", res_dijkstra_hanoi[fim_hanoi].distancia);
                        liberar_vetor_caminho_minimo(&res_dijkstra_hanoi);
                    }
                    else
                    {
                        printf("Nao foi possivel calcular o caminho com Dijkstra.\n");
                    }
                    break;
                }
                case 4:
                { // Bellman-Ford para Torre de Hanoi
                    int inicio_hanoi, fim_hanoi;
                    printf("Digite o indice do vertice inicial (0 a %d): ", MAX_HANOI - 1);
                    scanf("%d", &inicio_hanoi);
                    printf("Digite o indice do vertice final (0 a %d): ", MAX_HANOI - 1);
                    scanf("%d", &fim_hanoi);

                    if (inicio_hanoi < 0 || inicio_hanoi >= MAX_HANOI || fim_hanoi < 0 || fim_hanoi >= MAX_HANOI)
                    {
                        printf("Indices de vertice invalidos.\n");
                        break;
                    }
                    if (!is_valid_hanoi_config(configs_hanoi[inicio_hanoi]))
                    {
                        printf("Configuracao inicial (vertice %d) nao e valida para Torre de Hanoi.\n", inicio_hanoi);
                        break;
                    }
                    if (!is_valid_hanoi_config(configs_hanoi[fim_hanoi]))
                    {
                        printf("Configuracao final (vertice %d) nao e valida para Torre de Hanoi.\n", fim_hanoi);
                        break;
                    }

                    int tem_ciclo_negativo_hanoi = 0; // Bellman-Ford pode detectar ciclos
                    VETOR_CAMINHO_MINIMO *res_bf_hanoi = bellman_ford(&grafo_hanoi, inicio_hanoi, &tem_ciclo_negativo_hanoi);
                    if (tem_ciclo_negativo_hanoi)
                    {
                        printf("Bellman-Ford: Ciclo de peso negativo detectado. Isso nao deveria acontecer na Torre de Hanoi com pesos positivos.\n");
                    }
                    else if (res_bf_hanoi != NULL)
                    {
                        printf("\nCaminho Minimo com Bellman-Ford (Torre de Hanoi):\n");
                        int *predecessores_bf = (int *)malloc(grafo_hanoi.n_vertices * sizeof(int));
                        verificar_alocacao(predecessores_bf, "falha ao alocar predecessores para imprimir");
                        for (int i = 0; i < grafo_hanoi.n_vertices; ++i)
                        {
                            predecessores_bf[i] = res_bf_hanoi[i].vertice_anterior;
                        }
                        caminho_minimo_imprimir_sequencia(predecessores_bf, inicio_hanoi, fim_hanoi, grafo_hanoi.n_vertices);
                        free(predecessores_bf);

                        printf("Distancia minima: %d movimentos\n", res_bf_hanoi[fim_hanoi].distancia);
                        liberar_vetor_caminho_minimo(&res_bf_hanoi);
                    }
                    else
                    {
                        printf("Nao foi possivel calcular o caminho com Bellman-Ford.\n");
                    }
                    break;
                }
                case 0:
                    printf("Voltando ao Menu Principal...\n");
                    break;
                default:
                    printf("Opcao invalida!\n");
                }
            } while (sub_opcao != 0);

            liberar_grafo(&grafo_hanoi); // Libera o grafo da Torre de Hanoi
            break;
        }
        case 0:
            printf("Saindo do programa.\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao_menu_principal != 0);

    return 0;
}