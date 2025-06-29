#include "Arvore234/arvore234.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int percents[] = {10, 20, 35, 50};

int main() {
    arvore234* arvore = alocaArvore234();

    if (!arvore) {
        printf("Erro ao alocar memória para a árvore B.\n");
        return 1;
    }

    for(int i = 2; i <= 5; i++) {
        char filename[40];
        sprintf(filename, "Testes/Teste10e%d.txt", i);
        FILE *fp = fopen(filename, "r");

        if (!fp) {
            printf("Erro ao abrir o arquivo %s.\n", filename);
            free(arvore);
            return 1;
        }

        int chave;
        int *arr = (int *)malloc(sizeof(int) * (int)pow(10, i));

        printf("Teste com %d números:\n", (int)pow(10, i));

        for(int j = 0; j < (int)pow(10, i); j++) {
            fscanf(fp, "%d", &arr[j]);
            insereChaveArvore(arvore, arr[j]);
        }

        fclose(fp);

        printf("Inserção:\n");
        printf("Quantidade de splits: %d\n", obtemQtdSplit(arvore));
        printf("Quantidade de rotações: %d\n", obtemQtdRotacoes(arvore));
        printf("Altura da árvore: %d\n", calculaAltura234(arvore));

        for(int j = 0; j < 4; j++) {
            sprintf(filename, "Testes/Teste10e%d_%d.txt", i, percents[j]);
            fp = fopen(filename, "r");

            if (!fp) {
                printf("Erro ao abrir o arquivo %s.\n", filename);
                free(arr);
                free(arvore);
                return 1;
            }

            printf("Removendo %d%% dos números:\n", percents[j]);

            while(fscanf(fp, "%d", &chave) != EOF) {
                removeChaveArvore(arvore, chave);
            }

            fclose(fp);

            printf("Após remoção de %d%%:\n", percents[j]);
            printf("Quantidade de rotações: %d\n", obtemQtdRotacoes(arvore));
            printf("Quantidade de merges: %d\n", obtemQtdMerge(arvore));
            printf("Altura da árvore: %d\n", calculaAltura234(arvore));
            printf("\n");
        }

        printf("==============================================================\n\n");
    }
}