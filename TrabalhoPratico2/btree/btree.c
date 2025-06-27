#include "btree.h"

#include <stdio.h>
#include <stdlib.h>

#define ORDEM 4
#define MAXIMO_CHAVES (ORDEM - 1)
#define MAXIMO_FILHOS ORDEM

struct btree
{
    no* raiz;
    
    int qtdSplit;
    int altura;
};

struct no
{
    int qnt_chaves;
    int chaves[MAXIMO_CHAVES];
    no *filhos[MAXIMO_FILHOS];
    
    int folha;
};

btree* alocaBtree(){
    btree* novaArvore = (btree*)calloc(1, sizeof(btree));
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
    no* novoNo = calloc(1, sizeof(no));
    if(!novoNo)
        return NULL;

    novoNo->folha = folha;
    novoNo->qnt_chaves = 0;

    for (int i = 0; i < MAXIMO_CHAVES; i++){
        novoNo->chaves[i] = -1; // Inicializa as chaves com -1
    }

    return novoNo;
}

void insereBtree(btree* arv, int chave){
    printf("Inserindo chave: %d\n", chave);
    //Verificar se a árvore está vazia
    if(arv->raiz == NULL){
        no* novaRaiz = alocaNo(1);
        if(!novaRaiz)
            return;

        novaRaiz->chaves[0] = chave;
        novaRaiz->qnt_chaves = 1;

        arv->raiz = novaRaiz;
        return;
    }

    //Se a raiz estiver cheia, criar uma nova raiz
    if (arv->raiz->qnt_chaves == MAXIMO_CHAVES) {
        no* novaRaiz = alocaNo(0);
        if(!novaRaiz)
            return;

        //A nova raiz terá a antiga raiz como filho
        novaRaiz->filhos[0] = arv->raiz;

        //Dividir a antiga raiz
        dividirNo(arv, novaRaiz, 0);

        //Atualizar a raiz da árvore
        arv->raiz = novaRaiz;
    }

    //Inserir a chave na árvore
    insereChaveNaoCheio(arv, chave);
}

void insereChaveNaoCheio(btree* arv, int chave){
    //Encontrar o nó FOLHA para inserção
    no* aux = arv->raiz, *pai = NULL;
    int index = aux->qnt_chaves - 1; // Índice do último elemento no nó

    // Encontra o nó folha onde a chave deve ser inserida
    while (aux->folha == 0) {
        //Encontrar o filho correto para descer
        index = aux->qnt_chaves - 1; // Atualiza o índice para o novo nó
        
        while (index >= 0 && aux->chaves[index] >= chave) {
            index--;
        }

        index++; // Corrigir o índice para apontar para o filho correto

        pai = aux; // Guarda o nó pai
        aux = aux->filhos[index];
    }

    // Agora 'aux' é um nó folha onde a chave deve ser inserida
    if (aux->qnt_chaves == MAXIMO_CHAVES) {
        printf("Nó folha cheio, dividindo...\n");
        // Se o nó folha está cheio, dividir o nó
        dividirNo(arv, pai, index);

        // Após a divisão, verificar novamente onde inserir a chave
        if (chave > pai->chaves[index]) {
            aux = pai->filhos[index+1]; // Desce para o filho à direita
        }
    }

    // Inserir a chave no nó folha
    index = aux->qnt_chaves - 1;
    while (index >= 0 && aux->chaves[index] > chave) {
        aux->chaves[index + 1] = aux->chaves[index]; // Desloca as chaves para a direita
        index--;
    }

    aux->chaves[index + 1] = chave; // Insere a nova chave na posição correta

    aux->qnt_chaves++;
}

void dividirNo(btree *arv, no* pai, int index){
    printf("Dividindo nó no índice %d\n", index);
    no* filho = pai->filhos[index];
    no* novoNo = alocaNo(filho->folha);
    if(!novoNo)
        return;

    // Transferir metade das chaves do filho para o novo nó
    int meio = MAXIMO_FILHOS / 2 - 1; // Índice do meio
    novoNo->qnt_chaves = meio;
    printf("Meio: %d\n", meio);

    for (int i = 0; i < meio; i++) {
        novoNo->chaves[i] = filho->chaves[i + meio + 1]; // +1 para pular a chave do meio
    }

    // Atualizar o número de chaves no filho
    filho->qnt_chaves = meio;

    // Se o filho não é uma folha, transferir os filhos também
    if (!filho->folha) {
        for (int i = 0; i <= meio; i++) {
            novoNo->filhos[i] = filho->filhos[i + meio + 1];
        }
    }

    // Inserir a nova chave no pai
    for (int i = pai->qnt_chaves; i > index; i--) {
        pai->chaves[i] = pai->chaves[i - 1]; // Desloca as chaves para a direita
        pai->filhos[i + 1] = pai->filhos[i]; // Desloca os filhos para a direita
    }

    pai->chaves[index] = filho->chaves[meio]; // A chave do meio sobe para o pai
    pai->filhos[index + 1] = novoNo; // O novo nó é adicionado como filho

    pai->qnt_chaves++; // Incrementa o número de chaves no pai

    arv->qtdSplit++; // Incrementa o contador de splits
}

void percorrerBtree(no* raiz){
    if (raiz == NULL) {
        return;
    }

    // Percorre as chaves do nó atual
    for (int i = 0; i < raiz->qnt_chaves; i++) {
        printf("%d ", raiz->chaves[i]);
    }
    printf("\n");

    // Percorre os filhos do nó atual
    for (int i = 0; i <= raiz->qnt_chaves; i++) {
        percorrerBtree(raiz->filhos[i]);
    }
}

void imprimeBtree(btree *arv) {
    no *raiz = arv->raiz;

    if (raiz == NULL) {
        printf("A árvore está vazia.\n");
        return;
    }

    printf("[");
    for (int i = 0; i < raiz->qnt_chaves; i++) {
        printf("%d", raiz->chaves[i]);
        if (i < raiz->qnt_chaves - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    if (!raiz->folha) {
        for (int i = 0; i <= raiz->qnt_chaves; i++) {
            prettyPrintNode(raiz->filhos[i], "", i == raiz->qnt_chaves);
        }
    }
}

void prettyPrintNode(no *no, const char *prefix, int is_last) {
    if (no == NULL) {
        return;
    }

    printf("%s", prefix);
    printf(is_last ? "└── " : "├── ");
    
    printf("[");
    for (int i = 0; i < no->qnt_chaves; i++) {
        printf("%d", no->chaves[i]);

        if (i < no->qnt_chaves - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    if (!no->folha) {
        char new_prefix[1024];
        if(is_last)
            snprintf(new_prefix, sizeof(new_prefix), "%s    ", prefix);
        else
            snprintf(new_prefix, sizeof(new_prefix), "%s│   ", prefix);

        for (int i = 0; i <= no->qnt_chaves; i++) {
            prettyPrintNode(no->filhos[i], new_prefix, i == no->qnt_chaves);
        }
    }
}

int main(){
    btree* arv = alocaBtree();
    int chaves[] = {89,16,63,45,63,66,37,99,94,30,12,72,82,81,52};

    int tamanho = sizeof(chaves) / sizeof(chaves[0]);

    for (int i = 0; i < tamanho; i++) {
        insereBtree(arv, chaves[i]);
    }

    imprimeBtree(arv);
}