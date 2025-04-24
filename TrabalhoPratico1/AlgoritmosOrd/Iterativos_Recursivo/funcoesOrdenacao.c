// Desenvolvido por Gustavo Senador, revisado e comentado por Lucas Albuquerque
// 17/04/2025
// Universidade Federal de Itajubá - UNIFEI

//Bibliotecas;
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Biblioteca que possibilita trabalhar com tempo;
#include "funcoesOrdenacao.h"


//
void selectionSort(int* vetor, int tam, long long* comparacoes, long long* trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for(int i = 0; i < tam; i++) {
        int menor = i;
        for(int j = i + 1; j < tam; j++) {
            (*comparacoes)++;  // Cada vez que compara vetor[j] com vetor[menor]
            if(vetor[j] < vetor[menor]) {
                menor = j;
            }
        }

        if(i != menor) {
            int aux = vetor[i];
            vetor[i] = vetor[menor];
            vetor[menor] = aux;
            (*trocas)++;  // Conta a troca real
        }
    }
}

// Implementação do SelectionSort;
//
void insertionSort(int* vetor, int tam, long long* comparacoes, long long* trocas) {
    *comparacoes = 0;
    *trocas = 0;

    for (int i = 1; i < tam; i++) {
        int chave = vetor[i];
        int marcador = i - 1;

        // Fazemos a primeira comparação fora do loop para não perder nenhuma
        while (marcador >= 0 && vetor[marcador] > chave) {
            (*comparacoes)++;             // Comparação feita na condição
            vetor[marcador + 1] = vetor[marcador];
            (*trocas)++;                  // Contabiliza movimentação
            marcador--;
        }

        // Se saiu do while sem entrar, ainda assim teve uma comparação
        if (marcador >= 0)
            (*comparacoes)++;

        vetor[marcador + 1] = chave;
        (*trocas)++;  // Inserção da chave no lugar certo
    }
}

// Implementação do MergeSort;
//
void mergeSort(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas) {
    int meio = (inicio + fim) / 2;
    if (inicio < fim) {
        mergeSort(vetor, inicio, meio, comparacoes, trocas);
        mergeSort(vetor, meio + 1, fim, comparacoes, trocas);
        merge(vetor, inicio, meio, fim, comparacoes, trocas);
    }
}

void merge(int* vetor, int inicio, int meio, int fim, long long* comparacoes, long long* trocas) {
    int tamVetEsq = meio - inicio + 1;
    int* vetEsq = (int*)malloc(sizeof(int) * tamVetEsq);
    if (!vetEsq) return;

    int tamVetDir = fim - meio;
    int* vetDir = (int*)malloc(sizeof(int) * tamVetDir);
    if (!vetDir) {
        free(vetEsq);
        return;
    }

    int* vetAux = (int*)malloc(sizeof(int) * (tamVetEsq + tamVetDir));
    if (!vetAux) {
        free(vetEsq);
        free(vetDir);
        return;
    }

    for (int i = 0; i < tamVetEsq; i++)
        vetEsq[i] = vetor[i + inicio];
    for (int i = 0; i < tamVetDir; i++)
        vetDir[i] = vetor[meio + i + 1];

    int marcadorVetEsq = 0, marcadorVetDir = 0, marcadorVetAux = 0;

    while (marcadorVetEsq < tamVetEsq && marcadorVetDir < tamVetDir) {
        (*comparacoes)++;
        if (vetEsq[marcadorVetEsq] < vetDir[marcadorVetDir]) {
            vetAux[marcadorVetAux++] = vetEsq[marcadorVetEsq++];
        } else {
            vetAux[marcadorVetAux++] = vetDir[marcadorVetDir++];
        }
        (*trocas)++;  // Cada cópia para vetAux é uma movimentação
    }

    while (marcadorVetEsq < tamVetEsq) {
        vetAux[marcadorVetAux++] = vetEsq[marcadorVetEsq++];
        (*trocas)++;
    }

    while (marcadorVetDir < tamVetDir) {
        vetAux[marcadorVetAux++] = vetDir[marcadorVetDir++];
        (*trocas)++;
    }

    marcadorVetAux = 0;
    for (int i = inicio; i <= fim; i++) {
        vetor[i] = vetAux[marcadorVetAux++];
        (*trocas)++;  // Movimentação de volta ao vetor original
    }

    free(vetEsq);
    free(vetDir);
    free(vetAux);
}

#define TAMANHO_PILHA 1024  // Tamanho máximo da pilha auxiliar

// Função para trocar dois elementos de lugar
void trocar(int* a, int* b, long long* trocas) {
    int temp = *a;
    *a = *b;
    *b = temp;
    (*trocas)++;
}

int medianaDeCinco(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas) {
    int meio = inicio + (fim - inicio) / 2;
    int q1 = inicio + (meio - inicio) / 2;
    int q3 = meio + (fim - meio) / 2;

    if (vetor[inicio] > vetor[q1]) { (*comparacoes)++; trocar(&vetor[inicio], &vetor[q1], trocas); } else (*comparacoes)++;
    if (vetor[q1] > vetor[meio]) { (*comparacoes)++; trocar(&vetor[q1], &vetor[meio], trocas); } else (*comparacoes)++;
    if (vetor[meio] > vetor[q3]) { (*comparacoes)++; trocar(&vetor[meio], &vetor[q3], trocas); } else (*comparacoes)++;
    if (vetor[q3] > vetor[fim]) { (*comparacoes)++; trocar(&vetor[q3], &vetor[fim], trocas); } else (*comparacoes)++;
    if (vetor[inicio] > vetor[q1]) { (*comparacoes)++; trocar(&vetor[inicio], &vetor[q1], trocas); } else (*comparacoes)++;

    trocar(&vetor[meio], &vetor[fim - 1], trocas);
    return vetor[fim - 1];
}

int particiona(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas) {
    int pivo = medianaDeCinco(vetor, inicio, fim, comparacoes, trocas);
    int i = inicio;
    int j = fim - 1;

    while (1) {
        while (vetor[++i] < pivo) (*comparacoes)++;
        (*comparacoes)++;  // última comparação falsa

        while (vetor[--j] > pivo) (*comparacoes)++;
        (*comparacoes)++;

        if (i >= j) break;

        trocar(&vetor[i], &vetor[j], trocas);

        if (vetor[i] == pivo) { (*comparacoes)++; trocar(&vetor[i], &vetor[++inicio], trocas); } else (*comparacoes)++;
        if (vetor[j] == pivo) { (*comparacoes)++; trocar(&vetor[j], &vetor[--fim], trocas); } else (*comparacoes)++;
    }

    trocar(&vetor[i], &vetor[fim - 1], trocas);

    return i;
}

void quickSort(int* vetor, int inicio, int fim, long long* comparacoes, long long* trocas) {
    int* pilha = malloc(TAMANHO_PILHA * sizeof(int));
    if (pilha == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a pilha.\n");
        exit(EXIT_FAILURE);
    }

    int topo = -1;
    pilha[++topo] = inicio;
    pilha[++topo] = fim;

    while (topo >= 0) {
        fim = pilha[topo--];
        inicio = pilha[topo--];

        if (fim - inicio > 16) {
            int p = particiona(vetor, inicio, fim, comparacoes, trocas);

            if (p - inicio > fim - p) {
                pilha[++topo] = inicio;
                pilha[++topo] = p - 1;

                if (p + 1 < fim) {
                    pilha[++topo] = p + 1;
                    pilha[++topo] = fim;
                }
            } else {
                if (p + 1 < fim) {
                    pilha[++topo] = p + 1;
                    pilha[++topo] = fim;
                }

                pilha[++topo] = inicio;
                pilha[++topo] = p - 1;
            }
        }
    }

    free(pilha);
}
