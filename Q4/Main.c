#include <stdio.h>
#include <stdlib.h>
#include "Hash/Hash.h"

#define TAMANHO_VETOR_1 121 // Definindo o tamanho do vetor de funcionários
#define TAMANHO_VETOR_2 180 // Definindo o tamanho do vetor de funcionários para o segundo exemplo


int main()
{
    FUNCIONARIO *vetor = inicializar_vetor_funcionarios(TAMANHO_VETOR_1); // Inicializa o vetor de funcionários com tamanho 121

    FUNCIONARIO funcionario1 = criar_funcionario("123456", "Joao Silva", "Gerente", 5000.00);
    FUNCIONARIO funcionario2 = criar_funcionario("234567", "Maria Oliveira", "Analista", 4500.00);
    FUNCIONARIO funcionario3 = criar_funcionario("345678", "Carlos Pereira", "Desenvolvedor", 4000.00);

    //Funcionario 4 com matrícula repetida para testar colisão
    FUNCIONARIO funcionario4 = criar_funcionario("123456", "Ana Costa", "Gerente", 6000.00);
    FUNCIONARIO funcionario5 = criar_funcionario("123456", "Pedro Santos", "Analista", 4800.00);

    // Inserindo funcionários no vetor usando a função hash A
    short int resultado1 = inserir_funcionario_hash_A(vetor, TAMANHO_VETOR_1, funcionario1);
    short int resultado2 = inserir_funcionario_hash_A(vetor, TAMANHO_VETOR_1, funcionario2);
    short int resultado3 = inserir_funcionario_hash_A(vetor, TAMANHO_VETOR_1, funcionario3);
    short int resultado4 = inserir_funcionario_hash_A(vetor, TAMANHO_VETOR_1, funcionario4);
    short int resultado5 = inserir_funcionario_hash_A(vetor, TAMANHO_VETOR_1, funcionario5);

    // Verificando os resultados da inserção

    if (resultado1 > 0)
    {
        printf("Funcionario 1 inserido com sucesso\n");
    }
    else
    {
        printf("Erro ao inserir o Funcionario 1.\n");
    }


    if (resultado2 > 0)
    {
        printf("Funcionario 2 inserido com sucesso\n");
    }
    else
    {
        printf("Erro ao inserir o Funcionario 2.\n");
    }


    if (resultado3 > 0)
    {
        printf("Funcionario 3 inserido com sucesso\n");
    }
    else
    {
        printf("Erro ao inserir o Funcionario 3.\n");
    }


    if (resultado4 > 0)
    {
        printf("Funcionario 4 inserido com sucesso\n");
    }
    else
    {
        printf("Erro ao inserir o Funcionario 4.\n");
    }

    if (resultado5 > 0)
    {
        printf("Funcionario 5 inserido com sucesso\n");
    }
    else
    {
        printf("Erro ao inserir o Funcionario 5.\n");
    }

    // Imprimindo os funcionários inseridos
    printf("\nFuncionarios inseridos:\n");
    imprimir_vetor_funcionarios(vetor, TAMANHO_VETOR_1);

    return 0;
}