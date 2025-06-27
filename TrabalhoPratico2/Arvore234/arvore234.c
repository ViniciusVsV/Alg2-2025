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
    //Se a raiz estiver cheia, fazer a divisão dela
    if(arv->raiz->qtdChaves == maximoChaves){
        divideNo(arv, NULL, arv->raiz);

        arv->altura++;
    }
 
    no* aux = arv->raiz;
    no* auxPai = NULL;
    int index;

    //Percorrer a árvore até achar um nó folha
    while(aux->folha != 1) {
        index = maximoFilhos - 1;

        //Encontrar o filho para o qual deve-se percorrer
        for(int i = 0; i < maximoChaves; i++) {
            if(aux->chaves[i] == -1 || chave < aux->chaves[i]) {
                index = i;
                break;
            }
        }

        //Fazer a divisão preventiva no filho se estiver cheio
        if(aux->filhos[index]->qtdChaves == maximoChaves) {
            divideNo(arv, aux, aux->filhos[index]);

            if(chave > aux->chaves[index])
                index++;
        }

        //Descer ao filho
        auxPai = aux;
        aux = aux->filhos[index];
    }

    insereChaveNoFolha(aux, chave);
}

void divideNo(arvore234* arv, no* pai, no* noCheio){
    arv->qtdSplit++;

    //Se o pai for nulo, criar um novo nó para ele (pai da raiz atual)
    if(!pai){
        pai = alocaNo(0); //Não é folha 
        arv->raiz = pai;   //Atualiza a raiz
    }

    int meio = maximoChaves / 2;
    int intermediario = noCheio->chaves[meio];

    //Criar novo nó com os valores maiores que o intermediário
    no* noMaior = alocaNo(noCheio->folha); //O status de folha é igual ao do irmão

    //Transferir os valores de noCheio maiores que o valor intermediário para noMaior
    int j = 0;
    for(int i = meio + 1; i < maximoChaves; i++, j++){
        noMaior->chaves[j] = noCheio->chaves[i];
        noMaior->qtdChaves++;

        noCheio->chaves[i] = -1;
        noCheio->qtdChaves--;
    }

    //Transferir os ponteiros para dos filhos para noMaior
    if(noCheio->folha == 0){
        for(int i = meio + 1, k = 0; i <= maximoFilhos; i++, k++){
            noMaior->filhos[k] = noCheio->filhos[i];
            noCheio->filhos[i] = NULL;
        }
    }

    //Inserir o valor intermediário do noCheio no nó pai e atualizar os ponteiros da esquerda e direita para os filhos
    insereChaveNoInterno(pai, intermediario, noCheio, noMaior);

    noCheio->chaves[meio] = -1;
    noCheio->qtdChaves--;
}

void insereChaveNoFolha(no* noAlvo, int chave){
    printf("Inserindo elemento...\n");

    int i = noAlvo->qtdChaves - 1;
 
    //Encontrar a posição correta do vetor de chaves para inserção do novo elemento no nó
    for(i; i >= 0; i--){
        if(noAlvo->chaves[i] > chave)
            noAlvo->chaves[i + 1] = noAlvo->chaves[i];

        else if(noAlvo->chaves[i] < chave)
            break;
    }

    noAlvo->chaves[i + 1] = chave;

    noAlvo->qtdChaves++;
}

void insereChaveNoInterno(no* noAlvo, int chave, no* filhoEsquerdo, no* filhoDireito){
    int i = noAlvo->qtdChaves - 1;
 
    //Encontrar a posição correta do vetor de chaves para inserção do novo elemento no nó
    for(i; i >= 0; i--){
        if(noAlvo->chaves[i] > chave){
            noAlvo->chaves[i + 1] = noAlvo->chaves[i];
            noAlvo->filhos[i + 2] = noAlvo->filhos[i + 1];
        }

        else if(noAlvo->chaves[i] < chave)
            break;
    }

    noAlvo->chaves[i + 1] = chave;
    
    noAlvo->filhos[i + 1] = filhoEsquerdo;
    noAlvo->filhos[i + 2] = filhoDireito;

    noAlvo->qtdChaves++;
}

void percorreArvore(no* no){
    int i;

    if(no){
        for(i = 0; i < no->qtdChaves; i++){
            percorreArvore(no->filhos[i]);
        
            printf("%d\t", no->chaves[i]);
        }

        percorreArvore(no->filhos[i]);
    }
}

int main(){
    arvore234* arv = alocaArvore234();

    insereChaveArvore(arv, 1);
    insereChaveArvore(arv, 2);
    insereChaveArvore(arv, 3);
    insereChaveArvore(arv, 4);
    insereChaveArvore(arv, 5);
    insereChaveArvore(arv, 6);
    insereChaveArvore(arv, 7);
    insereChaveArvore(arv, 8);
    insereChaveArvore(arv, 9);
    insereChaveArvore(arv, 10);
    insereChaveArvore(arv, 11);
    insereChaveArvore(arv, 12);
    insereChaveArvore(arv, 13);
    insereChaveArvore(arv, 14);
    insereChaveArvore(arv, 15);
    insereChaveArvore(arv, 16);
    insereChaveArvore(arv, 17);
    insereChaveArvore(arv, 18);
    insereChaveArvore(arv, 19);
    insereChaveArvore(arv, 20);

    for (int i = 0; i <= arv->raiz->qtdChaves; i++) {
        for (int j = 0; j < arv->raiz->filhos[i]->qtdChaves; j++) {
            printf("%d\t", arv->raiz->filhos[i]->chaves[j]);
        }
        printf("\n");
    }

    printf("\nPercorrendo a árvore toda:\n");
    percorreArvore(arv->raiz);

    printf("\nAltura da árvore: %d\n", arv->altura);
    printf("Quantidade de splits: %d\n", arv->qtdSplit);
}