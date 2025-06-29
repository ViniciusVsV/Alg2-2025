#include "arvore234.h"

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////

int maximoChaves = 3;
int minimioChaves = 1;
int maximoFilhos = 4;
int minimioFilhos = 2;

struct arvore234{
    no234* raiz;
    
    int qtdSplit;
    int qtdMerge;
    int qtdRotacoes;
    int altura;
};

struct no234{
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
    novaArvore->qtdMerge = 0;
    novaArvore->qtdRotacoes = 0;
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

int preencheArvore(arvore234* arv, char* nomeArquivo){
    FILE* arquivo = fopen(nomeArquivo, "r");
    if(!arquivo)
        return 0;

    int chave;
    while(fscanf(arquivo, "%d\n", &chave) == 1)
        insereChaveArvore(arv, chave);

    return 1;
}

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

////////////////////////////////Métodos de Remoção de Elementos na Árvore/////////////////////////////////

void removeChaveArvore(arvore234 *arv, int chave){
    if(arv->raiz->qtdChaves == 0){
        printf("A árvore está vazia.\n");
        return;
    }

    removeChaveArvoreAux(arv, arv->raiz, chave);
}

void removeChaveArvoreAux(arvore234* arv, no234* noAtual, int chave){
    int i = 0;
    
    //Encontrar a posição da chave
    while(i < noAtual->qtdChaves && noAtual->chaves[i] < chave)
        i++;

    //Filho mais à direita
    if(i == noAtual->qtdChaves){
        if(!noAtual->folha)
            removeChaveArvoreAux(arv, noAtual->filhos[noAtual->qtdChaves], chave);
        
        printf("Elemento não encontrado\n");

        return;
    }
     
    //Chave é maior que a chave atual
    else if(noAtual->chaves[i] > chave){
        if(!noAtual->folha)
            removeChaveArvoreAux(arv, noAtual->filhos[i], chave);
        
        printf("Elemento não encontrado\n");

        return;
    }
    
    //Nó encontrado
    else{
        if(noAtual->folha){
            //Remover a chave e repara depois
            removeChaveNo(noAtual, chave);
            reparaRemocao(arv, noAtual);
        }
        else{
            //Encontrar predecessor
            no234* predecessor = noAtual->filhos[i];
            while(!predecessor->folha)
                predecessor = predecessor->filhos[predecessor->qtdChaves];
            
            //Substituir a chave pelo predecessor
            noAtual->chaves[i] = predecessor->chaves[predecessor->qtdChaves - 1];
            
            //Remover o predecessor da folha
            predecessor->qtdChaves--;
            
            //Reparar a árvore após a remoção
            reparaRemocao(arv, predecessor);

            printf("Elemento removido com sucesso\n");
        }
    }
}

void removeChaveNo(no234* no, int chave){
    int index = 0;

    //Encontrar a chave a ser removida
    while(index < no->qtdChaves && no->chaves[index] < chave)
        index++;

    //Se a chave não foi encontrada
    if(index == no->qtdChaves || no->chaves[index] != chave)
        return;

    //Deslocar as chaves para a esquerda
    for(int i = index; i < no->qtdChaves - 1; i++) 
        no->chaves[i] = no->chaves[i + 1];

    no->qtdChaves--;

    printf("Elemento removido com sucesso\n");
}

void reparaRemocao(arvore234* arv, no234* noAtual){
    //Se o nó tem chaves suficientes, não precisa reparar
    if(noAtual->qtdChaves >= minimioChaves)
        return;
    
    //Se é a raiz
    if(noAtual == arv->raiz) {
        if(noAtual->qtdChaves == 0){
            //Raiz vazia
            no234* raizAntiga = arv->raiz;

            if(!arv->raiz->folha)
                arv->raiz = arv->raiz->filhos[0];
            else 
                arv->raiz = NULL;
            
            free(raizAntiga);
        }

        return;
    }
    
    //Encontrar o pai e o índice do nó atual
    no234* pai = noAtual->pai;
    if(pai == NULL) return;
    
    int indicePai = 0;
    while(indicePai <= pai->qtdChaves && pai->filhos[indicePai] != noAtual)
        indicePai++;
    
    //Tentar emprestar do irmão da esquerda
    if(indicePai > 0 && pai->filhos[indicePai - 1]->qtdChaves > minimioChaves)
        emprestaEsquerda(arv, pai, indicePai);
    
    //Tentar emprestar do irmão da direita
    else if(indicePai < pai->qtdChaves && pai->filhos[indicePai + 1]->qtdChaves > minimioChaves)
        emprestaDireita(arv, pai, indicePai);
    
    //Juntar com um irmão
    else{
        no234* noMesclado;

        //Juntar com irmão da direita
        if(indicePai == 0)
            noMesclado = juntaNos(arv, pai, indicePai);

        //Juntar com irmão da esquerda
        else 
            noMesclado = juntaNos(arv, pai, indicePai - 1);
        
        // Reparar recursivamente o pai
        reparaRemocao(arv, pai);
    }
}

no234* emprestaEsquerda(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmaoEsquerdo = pai->filhos[index - 1];

    //Mover todas as chaves do filho uma posição para a direita
    for(int i = filho->qtdChaves - 1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];

    //Se não for folha, mover os filhos também
    if(!filho->folha){
        for(int i = filho->qtdChaves; i >= 0; i--)
            filho->filhos[i + 1] = filho->filhos[i];
        
        //O último filho do irmão esquerdo se torna o primeiro filho do filho
        filho->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->qtdChaves];
    }

    //A chave do pai desce para o filho
    filho->chaves[0] = pai->chaves[index - 1];

    //A última chave do irmão esquerdo sobe para o pai
    pai->chaves[index - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->qtdChaves - 1];

    //Atualizar contadores
    filho->qtdChaves++;
    irmaoEsquerdo->qtdChaves--;
    arv->qtdRotacoes++;
    
    return filho;
}

no234* emprestaDireita(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmaoDireito = pai->filhos[index + 1];

    //A chave do pai desce para o final do filho
    filho->chaves[filho->qtdChaves] = pai->chaves[index];

    //Se não for folha, o primeiro filho do irmão direito se torna o último filho do filho
    if(!filho->folha)
        filho->filhos[filho->qtdChaves + 1] = irmaoDireito->filhos[0];

    //A primeira chave do irmão direito sobe para o pai
    pai->chaves[index] = irmaoDireito->chaves[0];

    //Mover todas as chaves do irmão direito uma posição para a esquerda
    for(int i = 1; i < irmaoDireito->qtdChaves; i++)
        irmaoDireito->chaves[i - 1] = irmaoDireito->chaves[i];

    //Se não for folha, mover os filhos também
    if(!irmaoDireito->folha){
        for(int i = 1; i <= irmaoDireito->qtdChaves; i++)
            irmaoDireito->filhos[i - 1] = irmaoDireito->filhos[i];
    }

    //Atualizar contadores
    filho->qtdChaves++;
    irmaoDireito->qtdChaves--;
    arv->qtdRotacoes++;
    
    return filho;
}

no234* juntaNos(arvore234* arv, no234* pai, int index){
    no234* filho = pai->filhos[index];
    no234* irmao = pai->filhos[index + 1];

    //Chave do pai desce e se torna a nova chave no filho
    filho->chaves[filho->qtdChaves] = pai->chaves[index];

    // Copia chaves do irmão para o final do filho
    for(int i = 0; i < irmao->qtdChaves; i++)
        filho->chaves[filho->qtdChaves + 1 + i] = irmao->chaves[i];

    //Copia filhos do irmão
    if(!filho->folha){
        for(int i = 0; i <= irmao->qtdChaves; i++)
            filho->filhos[filho->qtdChaves + 1 + i] = irmao->filhos[i];
    }

    //Move chaves e filhos do pai para preencher o buraco
    for(int i = index + 1; i < pai->qtdChaves; i++){
        pai->chaves[i - 1] = pai->chaves[i];
        pai->filhos[i] = pai->filhos[i + 1];
    }

    //Atualiza o número de chaves no filho e no pai
    filho->qtdChaves += irmao->qtdChaves + 1;
    pai->qtdChaves--;

    filho->pai = pai;

    arv->qtdMerge++; //Incrementa o contador de merges

    return filho; //Retorna o nó que contém os elementos juntados
}

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////

no234* obtemRaiz234(arvore234* arv){
    return arv->raiz;
}
int obtemQtdSplit(arvore234* arv){
    return arv->qtdSplit;
}
int obtemQtdMerge(arvore234* arv){
    return arv->qtdMerge;
}
int obtemQtdRotacoes(arvore234* arv){
    return arv->qtdRotacoes;
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