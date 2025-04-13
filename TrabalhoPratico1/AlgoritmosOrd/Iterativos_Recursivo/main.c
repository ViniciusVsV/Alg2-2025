#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoesOrdenacao.h"

int main(){
    FILE* arq;
    arq = fopen("../Resultados/resultadosAlgoritmosAlternativos.csv" , "w");
    fprintf(arq , "Index,Tipo de Ordenação,Quantidade de posições de vetor,Tempo de execução(seg)\n");
    int index = 1;
    int quantidade_entrada[] = {2000 , 10000};

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        int* vetor = preencherVetor(quantidade_entrada[i]);
        double tempo = bubbleSort(vetor , quantidade_entrada[i]);
        printf("Ordenação de Nùmero %d / Tipo de ordenação: Bubble Sort / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Bubble Sort , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        int* vetor = preencherVetor(quantidade_entrada[i]);
        double tempo = bubbleSortInteligente(vetor , quantidade_entrada[i]);
       printf("Ordenação de Nùmero %d / Tipo de ordenação: Bubble Sort Inteligente / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Bubble Sort Inteligente , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        int* vetor = preencherVetor(quantidade_entrada[i]);
        double tempo = selectionSort(vetor , quantidade_entrada[i]);
        printf("Ordenação de Nùmero %d / Tipo de ordenação: Selection Sort / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Selection Sort , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        int* vetor = preencherVetor(quantidade_entrada[i]);
        double tempo = insertionSort(vetor , quantidade_entrada[i]);
        printf("Ordenação de Nùmero %d / Tipo de ordenação: Insertion Sort / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Insertion Sort , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        clock_t inicio , fim;
        inicio = clock();
    
        int* vetor = preencherVetor(quantidade_entrada[i]);
        mergeSort(vetor , 0 , quantidade_entrada[i] - 1);

        fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Ordenação de Nùmero %d / Tipo de ordenação: Merge Sort / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Merge Sort , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }

    for(int i = 0; i < (sizeof(quantidade_entrada) / sizeof(int)); i++){
        clock_t inicio , fim;
        inicio = clock();
    
        int* vetor = preencherVetor(quantidade_entrada[i]);
        quickSort(vetor , 0 , quantidade_entrada[i] - 1);

        fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Ordenação de Nùmero %d / Tipo de ordenação: Quick Sort / Quantidade de posições do vetor será ordenador %d \n" , index , quantidade_entrada[i]);
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Quick Sort , %d , %.7f \n" , index , quantidade_entrada[i] , tempo);
        index++;
    }
    return 0;
}