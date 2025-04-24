// Desenvolvido por Gustavo Senador, revisado e comentado por Lucas Albuquerque
// 17/04/2025
// Universidade Federal de Itajubá - UNIFEI

//Bibliotecas;
//
#include <stdio.h>
#include <stdlib.h>
#include "TreeSort.h"

// Definindo os dados do nó da árvore;
//
struct no{
    int valor;
    struct no* dir;
    struct no* esq;
};

// Definindo os dados da árvore;
//
struct arvore {
    no* raiz;
    int qtd;
};

// Aloca uma nova árvore;
//
arvore* criaArvore(){
    arvore* nova = (arvore *) malloc(sizeof(arvore));
    if(!nova) return NULL;

    nova->raiz = NULL;
    nova->qtd = 0;

    return nova;
}

// Aloca um novo nó com o seu valor passado por parâmetro;
//
no* criaNo(int valor){
    no* novo = (no *) malloc(sizeof(no));
    if(!novo) return NULL;

    novo->valor = valor;
    novo->dir = NULL;
    novo->esq = NULL;

    return novo;
}

// Insere um nó em uma árvore;
//
void inserirNo(arvore* arv , no* novo){
    if(!arv || !novo) return; // Previne erros na alocação de memória;

// Caso seja o primeiro elemento da árvore;
//
    if(!arv->raiz){
        arv->raiz = novo;
        arv->qtd++;
        return;
    }

// Percorrimento até encontrar a posição correta do elemento;
//
    no* atual = arv->raiz;
    no* ant = NULL;

    while(atual){
        ant = atual;

        if(atual->valor > novo->valor)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    if(novo->valor < ant->valor) {
        ant->esq = novo;
    }
    else {
        ant->dir = novo;
    }

    arv->qtd++;
}

// Percorrimento in-order utilizando a travessia de Morris, onde acontece a ordenação de fato;
//
void travessiaMorris(arvore *arv, int *vetOrdenado, long long* trocas, long long* comparacoes) {
    *comparacoes = 0;
    *trocas=0;
   // Armazena cada passo na árvore
    no *atual = arv->raiz; // Toda arvore será percorrida iniciando de sua raíz;
    no *predecessor;

    int i = 0; // Marca a posição atual do vetor;

    // Percorre toda a lista
    while (atual) {
// Caso o elemento seja o menor da sua sub-árvore;
//
        if (!atual->esq) {
            vetOrdenado[i] = atual->valor; // Armazena o elemento atual no vetor ordenado;
            i++;

            atual = atual->dir; // Passa para o próximo;
            (*comparacoes)++;
            (*trocas)++;
        }
        else {
// Busca o predecessor do nó atual;
//
            predecessor = atual->esq;
            while (predecessor->dir && predecessor->dir != atual) {
                predecessor = predecessor->dir;
                (*comparacoes)++;
            }

// Caso o predecessor não tenha ligação para o nó atual, essa ligação é criada temporariamente;
//
            if (!predecessor->dir) {
                predecessor->dir = atual;
                atual = atual->esq;

                (*comparacoes)++;
            }
            else {
                predecessor->dir = NULL; // Remoção do link temporário;
                vetOrdenado[i] = atual->valor; // Posiciona o elemento no vetor em sua posição correta;
                i++;

                atual = atual->dir; // Passa para o próximo;
                (*comparacoes)++;
                (*trocas)++;
            }
        }
    }
}

void executarTreeSort(int* vetor, int tam, long long* comparacoes, long long* trocas) {
    arvore* a = criaArvore();

    for (int i = 0; i < tam; i++) {
        no* n = criaNo(vetor[i]);
        inserirNo(a, n);  // passa contadores
    }

    int* vetOrdenado = malloc(sizeof(int) * tam);
    if (!vetOrdenado) {
        fprintf(stderr, "Erro de alocação para vetor ordenado.\n");
       
        return;
    }

    travessiaMorris(a, vetOrdenado,trocas,comparacoes  );  // só comparações aqui

    for (int i = 0; i < tam; i++) {
        vetor[i] = vetOrdenado[i];
    }

    free(vetOrdenado);
}
