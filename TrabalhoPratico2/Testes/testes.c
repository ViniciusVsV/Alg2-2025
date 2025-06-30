#include "../Arvore234/arvore234.h"
#include "../ArvoreRubroNegra/arvoreRB.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int porcentagens[] = {10, 20, 35, 50};

int main(){
    arvoreRB* arvRB = alocaArvoreRB();
    if(!arvRB){
        printf("Erro ao alocar memória para a árvore rubro-negra\n");
        return 1;
    }

    clock_t inicio, fim;
    double tempoGasto;
    char nomeArquivo[40];
    int chave;

    ///////////////////////////////////////////Testes da Árvore 2-3-4/////////////////////////////////////////////
    printf("TESTANDO ÁRVORE 2-3-4...\n\n");

    for(int i = 2; i <= 5; i++){
        arvore234* arv234 = alocaArvore234();
        if(!arv234){
            printf("Erro ao alocar memória para a árvore 2-3-4\n");
            return 1;
        }

        ////////////////////////////////////////////Teste de Inserção/////////////////////////////////////////////

        printf("TESTANDO INSERÇÃO COM %d NÚMEROS...\n\n", (int)pow(10, i));

        sprintf(nomeArquivo, "Testes/Numeros/Numeros_10e%d.txt", i);
        FILE *fp = fopen(nomeArquivo, "r");

        if(!fp){
            printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
            free(arv234);
            return 1;
        }

        inicio = clock();

        preencheArvore234(arv234, nomeArquivo);

        fclose(fp);

        fim = clock();
        tempoGasto = ((double)(fim - inicio) * 1000) / CLOCKS_PER_SEC;

        printf("Quantidade de splits: %d\n", obtemQtdSplit(arv234));
        printf("Altura da árvore: %d\n", calculaAltura234(arv234));
        printf("Quantidade de blocos: %d\n", obtemQtdNos(obtemRaiz234(arv234)));
        printf("Tempo gasto: %f\n\n", tempoGasto);

        ////////////////////////////////////////////Teste de Conversão////////////////////////////////////////////

        printf("TESTANDO CONVERSÃO...\n\n");

        inicio = clock();

        setRaiz(arvRB, converte234(obtemRaiz234(arv234), NULL));

        fim = clock();
        tempoGasto = ((double)(fim - inicio) * 1000) / CLOCKS_PER_SEC;

        printf("Tempo gasto: %f\n\n", tempoGasto);

        /////////////////////////////////////////////Teste de Remoção/////////////////////////////////////////////

        printf("TESTANDO REMOÇÃO...\n\n");

        for(int j = 0; j < 4; j++){
            sprintf(nomeArquivo, "Testes/Numeros/Numeros_10e%d_%d.txt", i, porcentagens[j]);
            fp = fopen(nomeArquivo, "r");

            if(!fp){
                printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
                free(arv234);
                return 1;
            }

            printf("%d%% DOS NÚMEROS:\n", porcentagens[j]);

            while(fscanf(fp, "%d", &chave) != EOF){
                removeChaveArvore(arv234, chave);
            }

            fclose(fp);

            printf("Quantidade de rotações: %d\n", obtemQtdRotacoes(arv234));
            printf("Quantidade de merges: %d\n", obtemQtdMerge(arv234));
            printf("Altura da árvore: %d\n", calculaAltura234(arv234));
            printf("Quantidade de blocos: %d\n", obtemQtdNos(obtemRaiz234(arv234)));
            printf("\n");
        }

        free(arv234);

        printf("==============================================================\n\n");
    }
}