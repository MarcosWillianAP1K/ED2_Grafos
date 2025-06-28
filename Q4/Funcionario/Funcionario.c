#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funcionario.h"
#include "../Utilitarios/Utilitarios.h"



    

FUNCIONARIO criar_funcionario(const char *matricula, const char *nome, const char *funcao, float salario)
{
    FUNCIONARIO funcionario;

    strncpy(funcionario.matricula, matricula, sizeof(funcionario.matricula) - 1);
    funcionario.matricula[sizeof(funcionario.matricula) - 1] = '\0'; // Garantir que a string esteja terminada

    strncpy(funcionario.nome, nome, sizeof(funcionario.nome) - 1);
    funcionario.nome[sizeof(funcionario.nome) - 1] = '\0'; // Garantir que a string esteja terminada

    strncpy(funcionario.funcao, funcao, sizeof(funcionario.funcao) - 1);
    funcionario.funcao[sizeof(funcionario.funcao) - 1] = '\0'; // Garantir que a string esteja terminada

    funcionario.salario = salario;
    return funcionario;
}

void imprimir_funcionario(const FUNCIONARIO *funcionario)
{
    printf("Matrícula: %s\n", funcionario->matricula);
    printf("Nome: %s\n", funcionario->nome);
    printf("Função: %s\n", funcionario->funcao);
    printf("Salário: %.2f\n", funcionario->salario);
}


FUNCIONARIO *inicializar_vetor_funcionarios(int tamanho)
{
    FUNCIONARIO *vetor = (FUNCIONARIO *)malloc(tamanho * sizeof(FUNCIONARIO));
    
    verificar_alocacao(vetor, "Erro ao alocar memória para o vetor de funcionários");

    for (int i = 0; i < tamanho; i++)
    {
        vetor[i] = criar_funcionario("", "", "", 0.0);
    }

    return vetor;
}