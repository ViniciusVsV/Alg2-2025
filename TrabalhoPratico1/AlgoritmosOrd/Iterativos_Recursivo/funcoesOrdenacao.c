// Desenvolvido por Gustavo Senador, revisado e comentado por Lucas Albuquerque
// 17/04/2025
// Universidade Federal de Itajubá - UNIFEI

//Bibliotecas;
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Biblioteca que possibilita trabalhar com tempo;
#include "funcoesOrdenacao.h"

// Implementação do BubbleSort em sua versão não otimizada;
//
double bubbleSort(int* vetor , int tamanho){
    // Variáveis utilizadas para calcular o tempo de execução;
    clock_t inicio = clock(); // clock() retorna o tempo atual;
    clock_t fim;
    double tempo;

// Comparações e trocas do BubbleSort;
//
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

// Cálculo do tempo com casting para double, garantindo maior precisão;
//
    tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

// Implementação do BubbleSort em sua versão otimizada;
//
double bubbleSortInteligente(int* vetor, int tamanho){
    // Variáveis utilizadas para calcular o tempo de execução;
    clock_t inicio = clock(); // clock() retorna o tempo atual;
    clock_t fim;
    double tempo;

    int trocou = 1; // Variável de controle com objetivo de evitar execuções desnecessárias;
    int i = 0;

//Comparações e trocas do BubbleSort;
//
    while(i < tamanho - 1 && trocou == 1) { // Percorrimento no vetor e verificação se está ou não ordenado;
        trocou = 0;
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
                trocou = 1;
            }
        }

        i++;
    }
    fim = clock();

// Cálculo do tempo com casting para double, garantindo maior precisão;
//
    tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

// Implementação do SelectionSort;
//
double selectionSort(int* vetor , int tam){
    // Variáveis utilizadas para calcular o tempo de execução;
    clock_t inicio = clock(); // clock() retorna o tempo atual;
    clock_t fim;
    double tempo;

    for(int i = 0; i < tam; i++) {
// Busca do menor elemento na parte desordenada do vetor;
//
        int menor = i;
        for(int j = i + 1; j < tam; j++) {
            if(vetor[j] < vetor[menor]) {
                menor = j;
            }
        }

// Troca do elemento i com o menor elemento, se necessário;
//
        if(i != menor) {
            int aux = vetor[i];
            vetor[i] = vetor[menor];
            vetor[menor] = aux;
        }
    }
    fim = clock();

// Cálculo do tempo com casting para double, garantindo maior precisão
//
    tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

// Implementação do SelectionSort;
//
double insertionSort(int* vetor , int tam){
    // Variáveis utilizadas para calcular o tempo de execução;
    clock_t inicio = clock(); // clock() retorna o tempo atual;
    clock_t fim;
    double tempo;

    for (int i = 1; i < tam; i++) {
        int chave = vetor[i]; // Elemento a ser realocado
        int marcador = i - 1;

        while (marcador >= 0 && vetor[marcador] > chave) { // Busca da posição do elemento na parte ordenada do vetor;
            vetor[marcador + 1] = vetor[marcador];
            marcador--;
        }

        vetor[marcador + 1] = chave;
    }
    fim = clock();

    //Cálculo do tempo com casting para double, garantindo maior precisão;
    tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;
    return tempo;
}

// Implementação do MergeSort;
//
void mergeSort(int*vetor , int inicio , int fim){
    int meio = (inicio + fim) / 2;
    if(inicio < fim){ // Condição de parada implícita, com objetivo de minimizar o uso do return;
        mergeSort(vetor , inicio , meio); // Divisão do lado esquerdo do vetor;
        mergeSort(vetor , meio + 1 , fim); // Divisão do lado direito do vetor;
        merge(vetor , inicio , meio ,  fim); // Junção dos vetores;
    }
}

void merge(int* vetor , int inicio , int meio , int fim){
// Alocação dos vetores;
//
    // Vetor da esquerda;
    int tamVetEsq = meio - inicio + 1;
    int* vetEsq = (int*)malloc(sizeof(int) * tamVetEsq);
    if(!vetEsq) return;

    // Vetor da direita;
    int tamVetDir = fim - meio;
    int* vetDir = (int*)malloc(sizeof(int) * tamVetDir);
    if(!vetDir) return;

    // Vetor auxiliar;
    int* vetAux = (int*)malloc(sizeof(int) * (tamVetEsq + tamVetDir));

// Preenchimento dos vetores
//
    for(int i = 0 ; i < tamVetEsq ; i++) { // Vetor da esquerda;
        vetEsq[i] = vetor[i + inicio];
    }

    for(int i = 0 ; i < tamVetDir ; i++){ // Vetor da direita;
        vetDir[i] = vetor[meio + i + 1];
    }

    // Marcadores de cada vetor;
    int marcadorVetEsq = 0;
    int marcadorVetDir = 0;
    int marcadorVetAux = 0;

// Preenche o vetor auxiliar em ordem até que algum dos outros tenha sido totalmente percorrido
//
    while(marcadorVetEsq < tamVetEsq && marcadorVetDir < tamVetDir){
        if(vetEsq[marcadorVetEsq] < vetDir[marcadorVetDir]){
            vetAux[marcadorVetAux] = vetEsq[marcadorVetEsq];
            marcadorVetEsq++;
            marcadorVetAux++;
        }
        else{
            vetAux[marcadorVetAux] = vetDir[marcadorVetDir];
            marcadorVetDir++;
            marcadorVetAux++;
        }
    }

// Preenche o vetor auxiliar com os elementos restante no vetor da esquerda, se necessário;
//
    while(marcadorVetEsq < tamVetEsq){
        vetAux[marcadorVetAux] = vetEsq[marcadorVetEsq];
        marcadorVetAux++;
        marcadorVetEsq++;
    }

// Preenche o vetor auxiliar com os elementos restante no vetor da direita, se necessário;
//
    while(marcadorVetDir < tamVetDir){
        vetAux[marcadorVetAux] = vetDir[marcadorVetDir];
        marcadorVetAux++;
        marcadorVetDir++;
    }

// Passa os elementos do vetor auxiliar para o vetor original com as posições ordenadas;
//
    marcadorVetAux = 0;
    for(int i = inicio; i <= fim ; i++){
        vetor[i] = vetAux[marcadorVetAux];
        marcadorVetAux++;
    }

// Liberação da memória alocada anteriormente;
//
    free(vetEsq);
    free(vetDir);
    free(vetAux);
}

// Implementação do QuickSort;
//
void quickSort(int* vetor , int inicio , int fim){
    if(inicio < fim){ // Condição de parada implícita, com objetivo de minimizar o uso do return;
        int pivo = particiona(vetor , inicio , fim); // Elemento já ordenado. Serve de parâmetro para as outras chamadas;
        quickSort(vetor , inicio , pivo - 1); // Ordenação na parte esquerda do vetor;
        quickSort(vetor , pivo + 1 , fim); // Ordenação na parte direita do vetor;
    }
}

// Busca a posição correta do pivô utilizado nas próximas chamadas e ordena o vetor parcialmente
//
int particiona(int* vetor , int inicio , int fim){
    int pos = inicio; // Marca a posição em que ficará o pivô;
    int aux;

//Busca a nova posição do pivô e posiciona os valores menores e maiores que ele a esquerda e direita, reespectivamente;
//
    for(int i = inicio + 1 ; i <= fim; i++){
        if(vetor[i] < vetor[inicio]){
            pos++;
            if(pos != i){ // Evita troca de um mesmo elemento;
                aux = vetor[i];
                vetor[i] = vetor[pos];
                vetor[pos] = aux;
            }
        }
    }

// Insere o pivo na sua posição já ordenado;
//
    aux = vetor[inicio];
    vetor[inicio] = vetor[pos];
    vetor[pos] = aux;

    return pos;
}
