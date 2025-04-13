#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoesOrdenacao.h"

int* preencherVetor(int quant){
    int* vetor = (int*)malloc(sizeof(int) * quant);

    srand(time(NULL));

    for(int i = 0 ; i < quant ; i++)
        vetor[i] = (rand() % 100) + 1;

    return vetor;
}

double bubbleSort(int* vetor , int tamanho){
    clock_t inicio , fim;
    inicio = clock();

    for(int i = 0; i < tamanho - 1; i++){
        for(int j = 0; j < tamanho - 1; j++){
            if(vetor[j] > vetor[j+1]){
                int temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
    }

    fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

double bubbleSortInteligente(int* vetor, int tamanho){
    int trocou;
    clock_t inicio,fim;

    inicio = clock();

    for (int i = 0; i < tamanho - 1; i++) {
        trocou = 0;
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                trocou = 1;
            }
        }
        if (trocou == 0)
            break;
    }

    fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

double selectionSort(int* vetor , int tam){
    clock_t inicio , fim;

    inicio = clock();

    for(int i = 0; i < tam; i++) {
        int marcador = i;
        int maior = vetor[i];
        int pos = i;
    
        for(int j = i + 1; j < tam; j++) {
            if(vetor[j] < maior) {
                maior = vetor[j];
                pos = j;
            }
        }
        if(marcador != pos) {
            int aux = vetor[marcador];
            vetor[marcador] = vetor[pos];
            vetor[pos] = aux;
        }
    }
    
    fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}
double insertionSort(int* vetor , int tam){
    clock_t inicio , fim;
    inicio = clock();

    for (int i = 1; i < tam; i++) {
        int chave = vetor[i];
        int j = i - 1;

        while ((j >= 0) && (vetor[j] > chave)) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }
    
    fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    return tempo;    
}
void merge(int* vetor , int inicio , int meio , int fim){
    int tamVetor1 = meio - inicio + 1;
    int* vetor1 = (int*)malloc(sizeof(int) * tamVetor1);
    for(int i = 0 ; i < tamVetor1 ; i++)
        vetor1[i] = vetor[i + inicio];
    
    int tamVetor2 = fim - meio;
    int* vetor2 = (int*)malloc(sizeof(int) * tamVetor2);
    for(int i = 0 ; i < tamVetor2 ; i++){
        vetor2[i] = vetor[meio + i + 1];
    }

    int* vetAux = (int*)malloc(sizeof(int) * (tamVetor1 + tamVetor2));

    int i = 0, j = 0 , contadorAux = 0;
    while((i < tamVetor1) && (j < tamVetor2)){
        if(vetor1[i] < vetor2[j]){
            vetAux[contadorAux] = vetor1[i];
            i++;
            contadorAux++;
        }
        else{
            vetAux[contadorAux] = vetor2[j];
            j++;
            contadorAux++;
        }
    }
    
    while(i < tamVetor1){
        vetAux[contadorAux] = vetor1[i];
        contadorAux++;
        i++;
    }

    while(j < tamVetor2){
        vetAux[contadorAux] = vetor2[j];
        contadorAux++;
        j++;
    }

    contadorAux = 0;
    for(int i = inicio ; i <= fim ; i++){
        vetor[i] = vetAux[contadorAux];
        contadorAux++;
    }
}

void mergeSort(int*vetor , int inicio , int fim){
    int meio = (inicio + fim) / 2;
    if(inicio < fim){
        mergeSort(vetor , inicio , meio);
        mergeSort(vetor , meio+1 , fim);
        merge(vetor , inicio , meio ,  fim);
    }
}

void quickSort(int* vetor , int inicio , int fim){
    if(inicio < fim){
        int pivo = particiona(vetor , inicio , fim);
        quickSort(vetor , inicio , pivo - 1);
        quickSort(vetor , pivo+1 , fim);
    }
}

int particiona(int* vetor , int inicio , int fim){
    int pivo = vetor[inicio];
    int pos = inicio;

    for(int i = inicio + 1 ; i <=fim ; i++){
        if(vetor[i] < pivo){
            pos++;
            if(pos != i){
                int aux = vetor[i];
                vetor[i] = vetor[pos];
                vetor[pos] = aux;
            }
        }
    }
    int aux = vetor[inicio];
    vetor[inicio] = vetor[pos];
    vetor[pos] = aux;

    return pos;
}
