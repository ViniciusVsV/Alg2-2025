#include "arvore234.h"

#include <stdio.h>
#include <stdlib.h>

int maximoChaves = 3;
int minimioChaves = 1;
int maximoFilhos = 4;
int minimioFilhos = 2;

struct arvore234
{
    no* raiz;
    
    int qtdSplit;
    int altura;
};

struct no
{
    int* chaves;
    no** filhos;
    
    int folha;
    int qtdChaves;
};

arvore234* alocaArvore234(){
    arvore234* novaArvore = (arvore234*) malloc(sizeof(arvore234));
    if(!novaArvore)
        return NULL;

    no* raiz = alocaNo(1);
    if(!raiz){
        free(novaArvore);

        return NULL;
    }

    novaArvore->qtdSplit = 0;
    novaArvore->altura = 0;

    novaArvore->raiz = raiz;

    return novaArvore;
}

no* alocaNo(int folha){
    no* novoNo = (no*) malloc(sizeof(no));
    if(!novoNo)
        return NULL;

    novoNo->chaves = (int*) malloc(maximoChaves * sizeof(int));
    novoNo->filhos = (no**) malloc(maximoFilhos * sizeof(no*));

    
    if(!novoNo->chaves || !novoNo->filhos){
        free(novoNo->chaves);
        free(novoNo->filhos);
        free(novoNo);

        return NULL;
    }

    for(int i = 0; i < maximoChaves; i++){
        novoNo->chaves[i] = -1;
        novoNo->filhos[i] = NULL;
    }
    novoNo->filhos[maximoFilhos - 1] = NULL;

    novoNo->folha = folha;
    novoNo->qtdChaves = 0;

    return novoNo;
}

void insereChaveArvore(arvore234* arv, int chave){
    //Encontrar o nó FOLHA para inserção
    no* aux = arv->raiz;

    while(aux->folha != 1){
        //Percorrer cada chave e verificar em qual filho entrar
        int index = maximoFilhos - 1;

        for(int i = 0; i < maximoChaves; i++){
            if(aux->chaves[i] == -1 || chave < aux->chaves[i]){
                index = i;
                
                break;
            }
        }

        aux = aux->filhos[index];
    }

    //Verificar se o nó está cheio
    //Se não estiver cheio, inserir
    if(aux->qtdChaves < maximoChaves)
        insereChaveNo(aux, chave);

    //Se estiver cheio, fazer o split
    else{
        printf("Nó cheio!\n");
    }
}

void insereChaveNo(no* no, int chave){
    //Encontrar a posição correta do vetor de chaves para inserção do novo elemento na folha
    int i = no->qtdChaves - 1;

    for(i; i >= 0; i--){
        if(no->chaves[i] > chave)
            no->chaves[i + 1] = no->chaves[i];

        else if(no->chaves[i] < chave)
            break;
    }

    no->chaves[i + 1] = chave;

    no->qtdChaves++;
}

void dividirNo(){

}

int main(){
    arvore234* arv = alocaArvore234();

    no* raiz = arv->raiz;

    for(int i = 0; i < maximoChaves; i++){
        printf("%d\t", raiz->chaves[i]);
    }printf("\n");
    for(int i = 0; i < maximoFilhos; i++){
        if(raiz->filhos[i])
            printf("Filho\t");
        else
            printf("Nulo\t");
    }printf("\n");

    insereChaveArvore(arv, 3);
    insereChaveArvore(arv, 2);
    insereChaveArvore(arv, 1);
    insereChaveArvore(arv, 4);

    printf("%d\n", raiz->qtdChaves);
    for(int i = 0; i < maximoChaves; i++){
        printf("%d\t", raiz->chaves[i]);
    }printf("\n");
    for(int i = 0; i < maximoFilhos; i++){
        if(raiz->filhos[i])
            printf("Filho\t");
        else
            printf("Nulo\t");
    }
}