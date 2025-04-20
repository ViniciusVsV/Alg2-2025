#ifndef FUNCOESORDENACAO
#define FUNCOESORDENACAO



void selectionSort(int* vetor, int tam, long long* comparacoes, long long* trocas);

void insertionSort(int* vetor, int tam, long long* comparacoes, long long* trocas);

void mergeSort(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas);
void merge(int* vetor, int inicio, int meio, int fim, long long* comparacoes, long long* trocas);


void trocar(int* a, int* b, long long* trocas);
int medianaDeCinco(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas);
void quickSort(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas);
int particiona(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas);

#endif  