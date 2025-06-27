
#pragma once


typedef struct FUNCIONARIO
{
    char matricula[7];// Matrícula do funcionário (6 dígitos + '\0')
    char nome[100];   // Nome do funcionário
    char funcao[100]; // Função do funcionário
    float salario;    // Salário do funcionário
} FUNCIONARIO;