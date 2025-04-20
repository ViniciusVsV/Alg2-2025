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
void inserirNo(arvore* arv, no* novo, long long* comparacoes, long long* trocas) {
    if (!arv || !novo) return;

    if (!arv->raiz) {
        arv->raiz = novo;
        arv->qtd++;
        (*trocas)++; // primeira inserção
        return;
    }

    no* atual = arv->raiz;
    no* ant = NULL;

    while (atual) {
        ant = atual;

        (*comparacoes)++;
        if (atual->valor > novo->valor)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    if (novo->valor < ant->valor)
        ant->esq = novo;
    else
        ant->dir = novo;

    (*trocas)++; // inserção
    arv->qtd++;
}

// Percorrimento in-order utilizando a travessia de Morris, onde acontece a ordenação de fato;
//
int travessiaMorris(arvore* arv, int* vetOrdenado, long long* comparacoes) {
    int i = 0;
    no* atual = arv->raiz;
    no* predecessor;

    while (atual) {
        if (!atual->esq) {
            vetOrdenado[i++] = atual->valor;
            atual = atual->dir;
        } else {
            predecessor = atual->esq;
            while (predecessor->dir && predecessor->dir != atual) {
                predecessor = predecessor->dir;
                (*comparacoes)++; // compara enquanto busca predecessor
            }

            if (!predecessor->dir) {
                predecessor->dir = atual;
                atual = atual->esq;
            } else {
                predecessor->dir = NULL;
                vetOrdenado[i++] = atual->valor;
                atual = atual->dir;
            }
        }
    }

    return i;
}

void executarTreeSort(int* vetor, int tam, long long* comparacoes, long long* trocas) {
    arvore* a = criaArvore();

    for (int i = 0; i < tam; i++) {
        no* n = criaNo(vetor[i]);
        inserirNo(a, n, comparacoes, trocas);  // passa contadores
    }

    int* vetOrdenado = malloc(sizeof(int) * tam);
    if (!vetOrdenado) {
        fprintf(stderr, "Erro de alocação para vetor ordenado.\n");
       
        return;
    }

    travessiaMorris(a, vetOrdenado, comparacoes);  // só comparações aqui

    for (int i = 0; i < tam; i++) {
        vetor[i] = vetOrdenado[i];
    }

    free(vetOrdenado);
}
