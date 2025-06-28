// Este código C gera arquivos contendo números ordenados aleatoriamente, 
// com tamanhos variados, e cria arquivos adicionais contendo porcentagens específicas desses números.
//
// Esses números são utilizados para testar a inserção e remoção em uma árvore B,
// permitindo avaliar o desempenho do algoritmo de árvore B em diferentes cenários de carga.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>

int arr[100000];

int percents[] = {10, 20, 35, 50};

int main() {
    // Gerar arquivos com 100, 1000, 1e4 e 1e5 números ordenados aleatoriamente

    mkdir("numeros", 0777);

    // Create directory for output files
    srand(time(NULL));

    FILE *fp;

    for (int i = 5; i > 1; i--) {
        printf("Gerando arquivo numeros_10e%d.txt\n", i);
        char filename[30];
        sprintf(filename, "numeros/numeros_10e%d.txt", i);
        fp = fopen(filename, "w");
        if (fp == NULL) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }

        for (int j = 0; j < pow(10, i); j++)
            arr[j] = j + 1;

        // Shuffle the array
        for (int j = 0; j < pow(10, i); j++) {
            int r = rand() % (int)(pow(10, i) - j) + j;
            int temp = arr[j];
            arr[j] = arr[r];
            arr[r] = temp;
        }

        for (int j = 0; j < pow(10, i); j++)
            fprintf(fp, "%d\n", arr[j]);

        fclose(fp);

        // Shuffle the array
        for (int j = 0; j < pow(10, i); j++) {
            int r = rand() % (int)(pow(10, i) - j) + j;
            int temp = arr[j];
            arr[j] = arr[r];
            arr[r] = temp;
        }

        // Get random 10%, 20%, 35%, and 50% of the numbers
        int last_percent = 0, last_index = 0;
        for (int percent = 0; percent < 4; percent++) {
            int num_elements = (int)(pow(10, i) * ((percents[percent] - last_percent) / 100.0));
            char filename_percent[40];
            sprintf(filename_percent, "numeros/numeros_10e%d_%d.txt", i, percents[percent]);
            fp = fopen(filename_percent, "w");
            if (fp == NULL) {
                perror("Erro ao abrir o arquivo");
                return 1;
            }

            for (int j = last_index; j < last_index + num_elements; j++)
                fprintf(fp, "%d\n", arr[j]);

            fclose(fp);
            last_percent = percents[percent];
            last_index += num_elements;
        }
    }

}