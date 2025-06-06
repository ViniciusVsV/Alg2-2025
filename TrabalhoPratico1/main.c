#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "funcoesOrdenacao.h"
#include "Geradoras.h"
#include "TreeSort.h"

// -------------------- Constantes de configuração --------------------

// Quantidade de repetições para casos aleatórios/quase ordenados
#define QTD_REPETICOES 5

// Número de tipos de vetores de entrada (ex: Aleatório, Crescente...)
#define QTD_TIPOS_ORDENACAO 4

// Quantidade de algoritmos de ordenação testados
#define QTD_ALGORITMOS 5

// Tamanhos dos vetores a serem testados    
int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};

// Quantidade de tamanhos a partir do array
int qtdTamanhos = sizeof(tamanhos) / sizeof(int);

// Tipos de ordenação (descrição para CSV e exibição)
char* tiposOrdenacao[] = {"Aleatorio", "Crescente", "Decrescente", "QuaseOrdenado"};
char* nomesColunas[] = {"Aleatório (média)", "Crescente", "Decrescente", "Quase ordenado (média)"};

// Nomes dos algoritmos utilizados (para título de arquivo, etc.)
char* algoritmos[] = {
    "SelectionSort", "InsertionSort", "MergeSort", "QuickSort", "TreeSort"
};



// -------------------- Geração dos vetores de teste --------------------

// Gera vetores com diferentes padrões
int* gerarVetor(int tipo, int tam, int semente) {
    switch (tipo) {
        case 0: return geraAleatorios(tam, semente);
        case 1: return geraOrdenados(tam, 0);        // Ordem crescente
        case 2: return geraOrdenados(tam, 1);        // Ordem decrescente
        case 3: return geraQuaseOrdenado(tam, 90);   // 90% ordenado
        default: return NULL;
    }
}

// -------------------- Execução dos algoritmos --------------------

// Seleciona e executa o algoritmo com base no índice
void executarAlgoritmo(int indice, int* vetor, int tam, long long* comparacoes, long long* trocas) {
    printf("Executando algoritmo %d (%s) com %d elementos...\n", indice, algoritmos[indice], tam);

    // Reinicia os contadores antes de executar
    *comparacoes = 0;
    *trocas = 0;

    switch (indice) {
        case 0:
            selectionSort(vetor, tam, comparacoes, trocas);
            break;
        case 1:
            insertionSort(vetor, tam, comparacoes, trocas);
            break;
        case 2:
            mergeSort(vetor, 0, tam - 1, comparacoes, trocas);
            break;
        case 3:
            quickSort(vetor, 0, tam - 1, comparacoes, trocas);  // <- chamada corrigida
            break;
        case 4:
            executarTreeSort(vetor, tam, comparacoes, trocas);
            break;
        default:
            printf("Algoritmo inválido!\n");
            return;
    }

    printf("Algoritmo %s finalizado.\n", algoritmos[indice]);
}


// -------------------- Benchmark --------------------

// Executa os testes de desempenho para um algoritmo específico e salva os resultados
void rodarBenchmarkAlgoritmo(int algoritmoIndice) {
    char nomeArquivo[100];
    sprintf(nomeArquivo, "benchmark_%s.csv", algoritmos[algoritmoIndice]);

    FILE* arq = fopen(nomeArquivo, "w");
    if (!arq) {
        perror("Erro ao abrir arquivo");
        return;
    }

    setvbuf(arq, NULL, _IOFBF, 8192);  // Buffer de escrita

    // Cabeçalho: Tempo, Comparações e Trocas para cada tipo de vetor
    fprintf(arq, "Tamanho Entrada");
    for (int i = 0; i < QTD_TIPOS_ORDENACAO; i++) {
        fprintf(arq, ";%s_Tempo(ms);%s_Comparacoes;%s_Trocas",
                nomesColunas[i], nomesColunas[i], nomesColunas[i]);
    }
    fprintf(arq, "\n");

    srand(time(NULL));  // Semente aleatória

    for (int t = 0; t < qtdTamanhos; t++) {
        int tam = tamanhos[t];
        fprintf(arq, "%d", tam);

        for (int tipo = 0; tipo < QTD_TIPOS_ORDENACAO; tipo++) {
            int repeticoes = (tipo == 0 || tipo == 3) ? QTD_REPETICOES : 1;

            double tempoTotal = 0.0;
            long long comparacoesTotal = 0;
            long long trocasTotal = 0;

            for (int r = 0; r < repeticoes; r++) {
                int semente = rand();
                int* vetor = gerarVetor(tipo, tam, semente);
                if (!vetor) {
                    fprintf(stderr, "Erro ao gerar vetor para tipo %d.\n", tipo);
                    continue;
                }

                long long comparacoes = 0;
                long long trocas = 0;

                clock_t ini = clock();
                executarAlgoritmo(algoritmoIndice, vetor, tam, &comparacoes, &trocas);
                clock_t fim = clock();

                tempoTotal += (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0;
                comparacoesTotal += comparacoes;
                trocasTotal += trocas;

                free(vetor);
            }

            double mediaTempo = tempoTotal / repeticoes;
            long long mediaComparacoes = comparacoesTotal / repeticoes;
            long long mediaTrocas = trocasTotal / repeticoes;

            fprintf(arq, ";%.6f;%lld;%lld", mediaTempo, mediaComparacoes, mediaTrocas);
        }

        fprintf(arq, "\n");
    }

    fclose(arq);
    printf("Benchmark gerado: %s\n", nomeArquivo);
}


// -------------------- Função principal --------------------

int main() {
    for (int i = 0; i < QTD_ALGORITMOS; i++) {
        rodarBenchmarkAlgoritmo(i);
    }

    return 0;
}
