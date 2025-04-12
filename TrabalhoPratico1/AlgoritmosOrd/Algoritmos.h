#ifndef Algoritmos
#define Algoritmos

void* salvaResultado();
//Função para salvar os resultados de cada algoritmo em seus respectivos arquivos
//Diminui repetição de código

void* insertionSort(int* vet, int tam);
//Implementação Insertion Sort

void* selectionSort(int* vet, int tam);
//Implementação Selection Sort

void* mergeSort(int* vet, int inicio, int fim);
//Implementação Merge Sort

void* quickSort(int* vet, int inicio, int fim);
//Implementação Quick Sort

void* treeSort();
//Implementação Tree Sort

#endif