#include "../Arvore234/arvore234.h"
#include "../ArvoreRubroNegra/arvoreRB.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

int porcentagens[] = {10, 20, 35, 50};

int main(){
    arvoreRB* arvRB = alocaArvoreRB();
    if(!arvRB){
        printf("Erro ao alocar memória para a árvore rubro-negra\n");
        return 1;
    }

    LARGE_INTEGER inicio, fim, frequencia;
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

        QueryPerformanceFrequency(&frequencia);
        QueryPerformanceCounter(&inicio);

        preencheArvore234(arv234, nomeArquivo);

        fclose(fp);

        QueryPerformanceCounter(&fim);
        tempoGasto = ((double)(fim.QuadPart - inicio.QuadPart) * 1000.0) / frequencia.QuadPart;

        printf("Quantidade de splits: %d\n", obtemQtdSplit(arv234));
        printf("Altura da árvore: %d\n", calculaAltura234(arv234));
        printf("Quantidade de blocos: %d\n", obtemQtdNos(obtemRaiz234(arv234)));
        printf("Tempo gasto: %lf\n\n", tempoGasto);

        ////////////////////////////////////////////Teste de Conversão////////////////////////////////////////////

        printf("TESTANDO CONVERSÃO...\n\n");

        QueryPerformanceFrequency(&frequencia);
        QueryPerformanceCounter(&inicio);

        setRaiz(arvRB, converte234(obtemRaiz234(arv234), NULL));

        QueryPerformanceCounter(&fim);
        tempoGasto = ((double)(fim.QuadPart - inicio.QuadPart) * 1000.0) / frequencia.QuadPart;

        printf("Tempo gasto: %lf\n", tempoGasto);
        printf("Altura da rubro-negra: %d\n\n", calculaAlturaRB(retornaRaizRB(arvRB)));

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