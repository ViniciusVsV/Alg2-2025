#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoesOrdenacao.h"
#include "Geradoras.h"
#include "TreeSort.h"

// Constantes para facilitar ajustes depois
#define QTD_REPETICOES 5                  // Quantas vezes repetir testes de entradas aleatórias/quase ordenadas
#define QTD_TIPOS_ORDENACAO 4            // Quantos tipos de entrada diferentes serão testados
#define QTD_ALGORITMOS 5                 // Quantos algoritmos de ordenação serão testados

// Tamanhos dos vetores que serão testados
int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
int qtdTamanhos = sizeof(tamanhos) / sizeof(int);  // Calcula a quantidade de tamanhos automaticamente

// Tipos de ordenação (como os vetores estarão organizados inicialmente)
char* tiposOrdenacao[] = {"Aleatorio", "Crescente", "Decrescente", "QuaseOrdenado"};
char* nomesColunas[] = {"Aleatório (média)", "Crescente", "Decrescente", "Quase ordenado (média)"};

// Nomes dos algoritmos que serão testados
char* algoritmos[] = {
    "SelectionSort", "InsertionSort", "MergeSort", "QuickSort", "TreeSort"
};

// Função auxiliar para executar o TreeSort, já que ele precisa de estrutura de árvore
void executarTreeSort(int* vetor, int tam) {
    lista* l = crialista();  // Cria a árvore (lista encadeada com árvore binária)
    for (int i = 0; i < tam; i++) {
        no* n = criano(vetor[i]);      // Cria um nó para cada elemento
        inserirLista(l, n);           // Insere na árvore
    }
    int idx = 0;
    int* vetOrdenado = malloc(sizeof(int) * tam);  // Vetor auxiliar para armazenar resultado
    emOrdem(l->pai, vetOrdenado, &idx);            // Faz o percurso em ordem e preenche vetor
    free(vetOrdenado);
    // Seria ideal liberar também a árvore (lista), mas não está feito aqui
}

// Gera um vetor de acordo com o tipo especificado
int* gerarVetor(int tipo, int tam, int semente) {
    switch (tipo) {
        case 0: return geraAleatorios(tam, semente);          // Aleatório
        case 1: return geraOrdenados(tam, 0);                 // Crescente
        case 2: return geraOrdenados(tam, 1);                 // Decrescente
        case 3: return geraQuaseOrdenado(tam, 90);            // Quase ordenado (90% ordenado)
        default: return NULL;
    }
}

// Chama o algoritmo de ordenação correto, de acordo com o índice
void executarAlgoritmo(int indice, int* vetor, int tam) {
    switch (indice) {
        case 0: selectionSort(vetor, tam); break;
        case 1: insertionSort(vetor, tam); break;
        case 2: mergeSort(vetor, 0, tam - 1); break;
        case 3: quickSort(vetor, 0, tam - 1); break;
        case 4: executarTreeSort(vetor, tam); break;
    }
}

// Executa os testes e salva os resultados de um algoritmo específico em um CSV
void rodarBenchmarkAlgoritmo(int algoritmoIndice) {
    char nomeArquivo[100];
    sprintf(nomeArquivo, "benchmark_%s.csv", algoritmos[algoritmoIndice]); // Nome do arquivo de saída
    FILE* arq = fopen(nomeArquivo, "w");

    if (!arq) {
        perror("Erro ao abrir arquivo");
        return;
    }

    // Otimiza escrita em arquivo usando buffer
    setvbuf(arq, NULL, _IOFBF, 8192);

    // Escreve o cabeçalho das colunas no CSV
    fprintf(arq, "Tamanho Entrada");
    for (int i = 0; i < QTD_TIPOS_ORDENACAO; i++) {
        fprintf(arq, ";%s", nomesColunas[i]);
    }
    fprintf(arq, "\n");

    srand(time(NULL));  // Semente randômica baseada no tempo atual

    for (int t = 0; t < qtdTamanhos; t++) {
        int tam = tamanhos[t];
        fprintf(arq, "%d", tam);

        // Pula tamanhos grandes para algoritmos lentos (Selection e Insertion)
        if ((algoritmoIndice == 0 || algoritmoIndice == 1) && tam >= 100000) {
            for (int i = 0; i < QTD_TIPOS_ORDENACAO; i++) {
                fprintf(arq, ";N/A");
            }
            fprintf(arq, "\n");
            continue;
        }

        for (int tipo = 0; tipo < QTD_TIPOS_ORDENACAO; tipo++) {
            // Testa várias vezes se o vetor for aleatório ou quase ordenado, para tirar média
            int repeticoes = (tipo == 0 || tipo == 3) ? QTD_REPETICOES : 1;
            double tempoTotal = 0.0;

            for (int r = 0; r < repeticoes; r++) {
                int semente = rand();  // Garante variação nas entradas aleatórias
                int* vetor = gerarVetor(tipo, tam, semente);  // Gera vetor de teste

                // Mede o tempo de execução
                clock_t ini = clock();
                executarAlgoritmo(algoritmoIndice, vetor, tam);
                clock_t fim = clock();

                tempoTotal += (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0;  // Converte para milissegundos
                free(vetor);  // Libera memória
            }

            double mediaTempo = tempoTotal / repeticoes;
            fprintf(arq, ";%.6f", mediaTempo);  // Salva tempo médio no CSV
        }

        fprintf(arq, "\n");
    }

    fclose(arq);  // Fecha o arquivo
    printf("Benchmark gerado: %s\n", nomeArquivo);  // Feedback no terminal
}

// Função principal: roda o benchmark para todos os algoritmos
int main() {
    for (int i = 0; i < QTD_ALGORITMOS; i++) {
        rodarBenchmarkAlgoritmo(i);
    }
    return 0;
}
