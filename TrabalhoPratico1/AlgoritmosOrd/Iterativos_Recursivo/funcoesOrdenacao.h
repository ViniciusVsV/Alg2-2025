#ifndef FUNCOESORDENACAO
#define FUNCOESORDENACAO

int* preencherVetor(int quant);

double bubbleSort(int* vetor , int tamanho);

double bubbleSortInteligente(int* vetor, int tamanho);

double selectionSort(int* vetor , int tam);

double insertionSort(int* vetor , int tam);

void mergeSort(int*vetor , int inicio , int fim);
void merge(int* vetor , int inicio , int meio , int fim);

void quickSort(int* vetor , int inicio , int fim);
int particiona(int* vetor , int inicio , int fim);

#endif