// Este código C gera arquivos contendo números ordenados aleatoriamente, 
// com tamanhos variados, e cria arquivos adicionais contendo porcentagens específicas desses números.
//
// Esses números são utilizados para testar a inserção e remoção em uma árvore B,
// permitindo avaliar o desempenho do algoritmo de árvore B em diferentes cenários de carga.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include <time.h>

int arr[100000];
int percents[] = {10, 20, 35, 50};

int main() {
    // Criação da pasta (sem permissão extra)
    if (_mkdir("Testes/Numeros") != 0) {
        // Ignora erro se já existe
        printf("Diretório 'Numeros' já existe ou não pôde ser criado.\n");
    }

    srand((unsigned)time(NULL));
    FILE *fp;

    for (int i = 5; i > 1; i--) {
        int tamanho = (int)pow(10, i);
        if (tamanho > 100000) {
            fprintf(stderr, "Tamanho excede limite do array: %d\n", tamanho);
            return 1;
        }

        printf("Gerando arquivo Teste10e%d.txt com %d elementos...\n", i, tamanho);

        char filename[40];
        sprintf(filename, "Testes/Numeros/Numeros_10e%d.txt", i);
        fp = fopen(filename, "w");
        if (fp == NULL) {
            perror("Erro ao abrir o arquivo principal");
            return 1;
        }

        // Preenche e embaralha
        for (int j = 0; j < tamanho; j++)
            arr[j] = j + 1;

        for (int j = 0; j < tamanho; j++) {
            int r = rand() % (tamanho - j) + j;
            int temp = arr[j];
            arr[j] = arr[r];
            arr[r] = temp;
        }

        for (int j = 0; j < tamanho; j++)
            fprintf(fp, "%d\n", arr[j]);

        fclose(fp);

        // Reembaralha para os percentuais
        for (int j = 0; j < tamanho; j++) {
            int r = rand() % (tamanho - j) + j;
            int temp = arr[j];
            arr[j] = arr[r];
            arr[r] = temp;
        }

        int last_percent = 0, last_index = 0;
        for (int p = 0; p < 4; p++) {
            int num_elements = (int)(tamanho * (percents[p] - last_percent) / 100.0);
            char filename_percent[50];
            sprintf(filename_percent, "Testes/Numeros/Numeros_10e%d_%d.txt", i, percents[p]);

            fp = fopen(filename_percent, "w");
            if (fp == NULL) {
                perror("Erro ao abrir o arquivo de percentual");
                return 1;
            }

            for (int j = last_index; j < last_index + num_elements; j++)
                fprintf(fp, "%d\n", arr[j]);

            fclose(fp);
            last_index += num_elements;
            last_percent = percents[p];
        }
    }

    printf("Todos os arquivos foram gerados com sucesso.\n");
    return 0;
}