#include "arvore234.h"

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////
int maximoChaves = 3;
int minimioChaves = 1;
int maximoFilhos = 4;
int minimioFilhos = 2;

struct arvore234
{
    no234* raiz;
    
    int qtdSplit;
    int altura;
};

struct no234
{
    int* chaves;
    no234** filhos;
    no234* pai;
    
    int folha;
    int qtdChaves;
};

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////
arvore234* alocaArvore234(){
    arvore234* novaArvore = (arvore234*) malloc(sizeof(arvore234));
    if(!novaArvore)
        return NULL;

    no234* raiz = alocaNo234(1);
    if(!raiz){
        free(novaArvore);

        return NULL;
    }

    novaArvore->qtdSplit = 0;
    novaArvore->altura = 0;

    novaArvore->raiz = raiz;

    return novaArvore;
}

no234* alocaNo234(int folha){
    no234* novoNo = (no234*) malloc(sizeof(no234));
    if(!novoNo)
        return NULL;

    novoNo->chaves = (int*) malloc(maximoChaves * sizeof(int));
    novoNo->filhos = (no234**) malloc(maximoFilhos * sizeof(no234*));
    novoNo->pai = NULL;
    
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

////////////////////////////////Métodos de Inserção de Elementos na Árvore////////////////////////////////
void insereChaveArvore(arvore234* arv, int chave){
    no234* aux = arv->raiz;
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

        //Descer ao filho
        aux = aux->filhos[index];
    }

    //Encontrado o nó folha, verificar se está cheio
    //Se está cheio, realizar a divisão e então inserir
    if(aux->qtdChaves == maximoChaves){
        aux = divideNo(arv, aux->pai, aux, chave);

        insereChaveNoFolha(aux, chave);
    }

    //Se não está cheio, apenas inserir
    else{
        insereChaveNoFolha(aux, chave);
    }
}

no234* divideNo(arvore234* arv, no234* pai, no234* noCheio, int chave){
    arv->qtdSplit++;

    //Se o pai for nulo, criar um novo nó para ele (pai da raiz atual)
    if(!pai){
        pai = alocaNo234(0); //Não é folha 
        arv->raiz = pai;   //Atualiza a raiz

        noCheio->pai = pai; //No cheio aponto para o novo pai

        arv->altura++;
    }

    int meio = maximoChaves / 2;
    int intermediario = noCheio->chaves[meio];

    //Criar novo nó com os valores maiores que o intermediário
    no234* noMaior = alocaNo234(noCheio->folha); //O status de folha é igual ao do irmão
    noMaior->pai = pai; //No maior aponta para o pai novo ou já existente

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
    
    //Se o pai estiver cheio, chamar recursão
    if(pai->qtdChaves == maximoChaves){
        pai = divideNo(arv, pai->pai, pai, chave);

        //Atualizar os ponteiros apra o pai dos nós cheio e maior
        noCheio->pai = pai;
        noMaior->pai = pai;
    }

    //Inserir o valor intermediário do noCheio no nó pai e atualizar os ponteiros
    insereChaveNoInterno(pai, intermediario, noCheio, noMaior);

    noCheio->chaves[meio] = -1;
    noCheio->qtdChaves--;

    if(chave < intermediario)
        return noCheio;
    else
        return noMaior;
}

void insereChaveNoFolha(no234* noAlvo, int chave){
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

void insereChaveNoInterno(no234* noAlvo, int chave, no234* filhoEsquerdo, no234* filhoDireito){
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

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////
no234* obtemRaiz234(arvore234* arv){
    return arv->raiz;
}
int obtemQtdSplit(arvore234* arv){
    return arv->qtdSplit;
}
int obtemAltura234(arvore234* arv){
    return arv->altura;
}
int* obtemChaves(no234* no){
    return no->chaves;
}
no234** obtemFilhos(no234* no){
    return no->filhos;
}
int obtemQtdChaves(no234* no){
    return no->qtdChaves;
}

void imprimeArvore234(arvore234 *arv){
    no234* raiz = arv->raiz;

    if(raiz == NULL){
        printf("A árvore está vazia.\n");
        return;
    }

    printf("[");
    for(int i = 0; i < raiz->qtdChaves; i++){
        printf("%d", raiz->chaves[i]);

        if(i < raiz->qtdChaves - 1) 
            printf(", ");
    }
    printf("]\n");

    if(!raiz->folha){
        for(int i = 0; i <= raiz->qtdChaves; i++) 
            imprimeNo234(raiz->filhos[i], "", i == raiz->qtdChaves);
    }
}

void imprimeNo234(no234* no, const char *prefixo, int is_last){
    if(no == NULL) 
        return;

    printf("%s", prefixo);
    printf(is_last ? "└── " : "├── ");
    
    printf("[");
    for(int i = 0; i < no->qtdChaves; i++){
        printf("%d", no->chaves[i]);

        if(i < no->qtdChaves - 1)
            printf(", ");
    }
    printf("]\n");

    if(!no->folha){
        char novoPrefixo[1024];
        if(is_last)
            snprintf(novoPrefixo, sizeof(novoPrefixo), "%s    ", prefixo);
        else
            snprintf(novoPrefixo, sizeof(novoPrefixo), "%s│   ", prefixo);

        for(int i = 0; i <= no->qtdChaves; i++) 
            imprimeNo234(no->filhos[i], novoPrefixo, i == no->qtdChaves);
    }
}