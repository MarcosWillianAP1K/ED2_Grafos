
#pragma once


typedef struct FUNCIONARIO
{
    char matricula[7];// Matrícula do funcionário (6 dígitos + '\0')
    char nome[100];   // Nome do funcionário
    char funcao[100]; // Função do funcionário
    float salario;    // Salário do funcionário
} FUNCIONARIO;


FUNCIONARIO *inicializar_vetor_funcionarios(int tamanho);

FUNCIONARIO criar_funcionario(const char *matricula, const char *nome, const char *funcao, float salario);

void imprimir_funcionario(const FUNCIONARIO funcionario);

void imprimir_vetor_funcionarios(const FUNCIONARIO *vetor, int tamanho);