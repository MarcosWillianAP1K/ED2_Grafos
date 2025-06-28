#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Hash.h"

void rotacao_esquerda(char *str)
{
    int tamanho = strlen(str); // Obtém o tamanho da string

    if (tamanho > 1)
    {
        char primeiro = str[0]; // Salva o primeiro caractere

        // Move todos os caracteres para a esquerda
        for (int i = 0; i < tamanho - 1; i++)
        {
            str[i] = str[i + 1];
        }

        str[tamanho - 1] = primeiro; // Coloca o primeiro caractere no final
    }
}

// Conta a partir de 1
char extrair_caractere(const char *str, int posicao)
{
    int tamanho = strlen(str);
    if (posicao < 1 || posicao > tamanho)
    {
        return '\0'; // Retorna nulo se a posição for inválida
    }

    return str[posicao - 1];
}

int resto_divisao(int a, int b)
{
    if (b == 0)
    {
        fprintf(stderr, "Erro: Divisão por zero.\n");
        exit(EXIT_FAILURE);
    }
    return a % b;
}

// Esse fold shift é especifico para a questão 4 letra B do trabalho de ED2
void fold_shift(const char *entrada, char *saida)
{
    // Extração dos caracteres

    char conjunto1[3] = {0};
    char conjunto2[3] = {0};
    char conjunto3[3] = {0};

    // Extração dos caracteres de acordo com a posição
    conjunto1[0] = extrair_caractere(entrada, 1);
    conjunto1[1] = extrair_caractere(entrada, 2);
    conjunto1[2] = '\0'; // Null terminator

    conjunto2[0] = extrair_caractere(entrada, 6);
    conjunto2[1] = extrair_caractere(entrada, 3);
    conjunto2[2] = '\0'; // Null terminator

    conjunto3[0] = extrair_caractere(entrada, 4);
    conjunto3[1] = extrair_caractere(entrada, 5);
    conjunto3[2] = '\0'; // Null terminator

    // Conversão para inteiros e soma dos valores

    int valor1 = atoi(conjunto1);
    int valor2 = atoi(conjunto2);
    int valor3 = atoi(conjunto3);

    int soma = valor1 + valor2 + valor3;

    // Retirar primeiro digito caso ultrupassar 3 digitos

    if (soma > 999)
    {
        // Remove o primeiro dígito
        int primeiro_digito = soma / 1000;
        soma -= primeiro_digito * 1000;
    }

    // Conversão da soma para string
    sprintf(saida, "%d", soma);
    saida[3] = '\0'; // Garantir que a string esteja terminada
}

int tratar_colisao(int resultado, int digito)
{
    return resultado + digito; // Soma o dígito ao resultado
}

void funcao_hash_A(FUNCIONARIO *vetor, int tamanho, FUNCIONARIO inserir)
{
    char matricula[7];
    strncpy(matricula, inserir.matricula, sizeof(matricula) - 1);
    matricula[sizeof(matricula) - 1] = '\0'; // Garantir que a string esteja terminada

    // Rotação duas vezes para a esquerda
    rotacao_esquerda(matricula); // Rotaciona a matrícula para a esquerda
    rotacao_esquerda(matricula); // Rotaciona novamente para a esquerda

    char numeros_extraidos[4] = {0};

    numeros_extraidos[0] = extrair_caractere(matricula, 2);
    numeros_extraidos[1] = extrair_caractere(matricula, 4);
    numeros_extraidos[2] = extrair_caractere(matricula, 5);
    numeros_extraidos[3] = '\0'; // Null terminator

    int resultado = atoi(numeros_extraidos); // Converte os caracteres extraídos para inteiro

    resultado = resto_divisao(resultado, tamanho); // Calcula o resto da divisão pelo tamanho do vetor

    short int valido = 1;               // Variável para verificar se a posição está livre
    short int loop = 0;                 // Variável para controlar o loop
    int resultado_original = resultado; // Armazena o resultado original para verificar colisões

    while (valido)
    {
        int aux = resultado; // Armazena o resultado atual

        if (resultado >= tamanho) // Verifica se o resultado é maior ou igual ao tamanho do vetor
        {
            aux = resultado - ((resultado / tamanho) * tamanho); // Se for, redefine para 0

            if (loop == 1) // Se for a primeira iteração, redefine o resultado para 0
            {
                valido = 0;

                //Sobreescreve no resultado o valor original e o loop finaliza
                resultado = resultado_original;
            }

            loop = 1; // Define que já foi feita uma iteração
        }
        else
        {

            if (vetor[aux].matricula[0] == '\0') // Verifica se a posição está livre
            {
                vetor[aux] = inserir; // Insere o funcionário na posição livre
                valido = 0;           // Define como inválido para sair do loop
            }
            else
            {
                char primeiro_digito = extrair_caractere(vetor[aux].matricula, 1); // Extrai o primeiro dígito da matrícula
                resultado = tratar_colisao(resultado, atoi(&primeiro_digito)); // Trata colisão somando o primeiro dígito da matrícula
            }
        }
    }


    vetor[resultado] = inserir; // Insere o funcionário na posição final
}
