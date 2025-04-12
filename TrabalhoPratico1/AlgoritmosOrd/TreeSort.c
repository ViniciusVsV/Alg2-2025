#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TreeSort.h"

struct no{
    int dado;
    struct no* direita;
    struct no* esquerda;
};


lista* crialista(){
    lista* nova = (lista*)malloc(sizeof(lista));
    if(nova == NULL) return NULL;

    nova->pai = NULL;
    nova->quant = 0;

    return nova;
}

no* criano(int dado){
    no* novo = (no*)malloc(sizeof(no));
    if(novo == NULL) return NULL;

    novo->dado = dado;
    novo->direita = NULL;
    novo->esquerda = NULL;

    return novo;
}

void inserirLista(lista* lista , no* novo){
    if((lista == NULL) || (novo == NULL)) return;

    if(lista->pai == NULL){
        lista->pai = novo;
        lista->quant++;
        return;
    }

    no* atual = lista->pai;
    no* ant = NULL;

    while(atual != NULL){
        ant = atual;

        if(atual->dado > novo->dado)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    if(novo->dado < ant->dado)
        ant->esquerda = novo;
    
    else
        ant->direita = novo;
    
    lista->quant++;
    return;
}

int* preencherVetor(int quant){
    int* vetor = (int*)malloc(sizeof(int) * quant);

    srand(time(NULL));

    for(int i = 0 ; i < quant ; i++)
        vetor[i] = (rand() % 100) + 1;

    return vetor;
}

void emOrdem(no* raiz , int* vetOrdem , int* VetIndex){
    if(raiz == NULL) return;

    emOrdem(raiz->esquerda , vetOrdem , VetIndex);

    vetOrdem[*VetIndex] = raiz->dado;
    (*VetIndex)++;

    emOrdem(raiz->direita , vetOrdem , VetIndex);
}



