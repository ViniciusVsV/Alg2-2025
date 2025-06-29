#include "arvoreRB.h"

#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////

struct arvoreRB{
    noRB* sentinela;
};

struct noRB{
    int chave;
    char cor;

    noRB* esq;
    noRB* dir;
    noRB* pai;
};

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////

arvoreRB* alocaArvoreRB(){
    arvoreRB* novaArvore = (arvoreRB*) malloc(sizeof(arvoreRB));
    if(!novaArvore) return NULL;

    noRB* sentinela = alocaNoRB(-100000, 'P');
    if(!sentinela) return NULL;

    novaArvore->sentinela = sentinela;

    return novaArvore;
}

noRB* alocaNoRB(int chave, char cor){
    noRB* novoNo = (noRB*) malloc(sizeof(noRB));
    if(!novoNo) return NULL;

    novoNo->chave = chave;
    novoNo->cor = cor;

    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->pai = NULL;

    return novoNo;
}

//////////////////////////Métodos de Inserção e Remoção de Elementos na Árvore////////////////////////////

void insereNoRB(arvoreRB* arv, noRB* novoNo){
    // Variáveis que percorrerão a árvore;
    noRB* aux = arv->sentinela->dir;
    noRB* auxPai = arv->sentinela;

    // Procura pela posição correta para inserção;
    while(aux){
        auxPai = aux;

        if(novoNo->chave < aux->chave)
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    if(novoNo->chave < auxPai->chave) // Caso o for inserido a esquerda de seu pai;
        auxPai->esq = novoNo;
    else 
        auxPai->dir = novoNo; // Caso o for inserido a direita de seu pai;

    novoNo->pai = auxPai;

    balanceamentoInsercao(arv, novoNo);
}

int removeNoRB(arvoreRB* arv, int valor){
    noRB* aux = arv->sentinela->dir;
    char corRemovido;

    // Procura pelo nó a ser removido;
    while(aux && aux->chave != valor){
        if(valor < aux->chave) 
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    if(!aux) return 0; // Nó não encontrado;

    corRemovido = aux->cor;

    // Caso o nó removido for folha;
    if(!aux->esq && !aux->dir){
        if(aux->pai->esq == aux)
            aux->pai->esq = NULL;
        else
            aux->pai->dir = NULL;

        if(corRemovido == 'P')
            balanceamentoRemocao(arv, NULL, aux->pai);
    }

    // Caso o nó removido contenha os 2 filhos
    else if(aux->esq && aux->dir){
        noRB* predecessor = aux->esq;

        //Busca pelo PREDECESSOR;
        while(predecessor->dir)
            predecessor = predecessor->dir;

        aux->chave = predecessor->chave; //Transferência de dados ao predecessor;

        // Atualização dos ponteiros;
        if(predecessor->pai->esq == predecessor)
            predecessor->pai->esq = predecessor->esq;
        else
            predecessor->pai->dir = predecessor->esq;

        if(predecessor->esq) predecessor->esq->pai = predecessor->pai;

        if(predecessor->cor == 'P')
            balanceamentoRemocao(arv, predecessor->esq, predecessor->pai);

        aux = predecessor;
    }

    // Caso o nó removido contenha apenas o filho a esquerda;
    else if(aux->esq){
        // Atualização de ponteiros;
        if(aux->pai->esq == aux)
            aux->pai->esq = aux->esq;
        else
            aux->pai->dir = aux->esq;

        aux->esq->pai = aux->pai;
        
        if(aux->cor == 'P')
            balanceamentoRemocao(arv, aux->esq, aux->pai);
    }

    // Caso o nó removido contenha apenas o filho a direita;
    else if(aux->dir){
        // Atualização de ponteiros;
        if(aux->pai->esq == aux)
            aux->pai->esq = aux->dir;
        else
            aux->pai->dir = aux->dir;

        aux->dir->pai = aux->pai;

        if(aux->cor == 'P')
            balanceamentoRemocao(arv, aux->dir, aux->pai);
    }

    free(aux); // Liberação de memória do nó removido;
    return 1;
}

//////////////////////Algoritmo de Conversão da Árvore 2-3-4 em Árvore Rubro-Negra////////////////////////

noRB* converte234(no234* raiz234, noRB* pai){
    if(!raiz234)
        return NULL;

    noRB* raizRB = NULL;

    int qtdChaves = obtemQtdChaves(raiz234);
    int* chaves = obtemChaves(raiz234);
    no234** filhos = obtemFilhos(raiz234);

    //Nó 2 -> um nó preto
    if (qtdChaves == 1){
        raizRB = alocaNoRB(chaves[0], 'P');
        raizRB->pai = pai;

        raizRB->esq = converte234(filhos[0], raizRB);
        raizRB->dir = converte234(filhos[1], raizRB);
    }

    //Nó 3 -> um nó preto com filho vermelho à direita
    else if (qtdChaves == 2){
        raizRB = alocaNoRB(chaves[0], 'P');
        raizRB->pai = pai;

        noRB* vermelho = alocaNoRB(chaves[1], 'V');
        vermelho->pai = raizRB;

        raizRB->esq = converte234(filhos[0], raizRB);
        raizRB->dir = vermelho;

        vermelho->esq = converte234(filhos[1], vermelho);
        vermelho->dir = converte234(filhos[2], vermelho);
    }

    //Nó 4 -> um nó preto com dois filhos vermelhos
    else if (qtdChaves == 3){
        raizRB = alocaNoRB(chaves[1], 'P');
        raizRB->pai = pai;

        noRB* vermelhoEsq = alocaNoRB(chaves[0], 'V');
        vermelhoEsq->pai = raizRB;

        noRB* vermelhoDir = alocaNoRB(chaves[2], 'V');
        vermelhoDir->pai = raizRB;

        raizRB->esq = vermelhoEsq;
        raizRB->dir = vermelhoDir;

        vermelhoEsq->esq = converte234(filhos[0], vermelhoEsq);
        vermelhoEsq->dir = converte234(filhos[1], vermelhoEsq);

        vermelhoDir->esq = converte234(filhos[2], vermelhoDir);
        vermelhoDir->dir = converte234(filhos[3], vermelhoDir);
    }

    return raizRB;
}

///////////////////////////////////Métodos de Balanceamento da Árvore/////////////////////////////////////

void balanceamentoInsercao(arvoreRB* arv, noRB* novoNo){
    noRB *aux = novoNo;
    noRB *pai = aux->pai;
    noRB *tio = NULL;
    noRB *avo = pai->pai;
    
    // Avalia o nó enquanto ele não for a raiz seu pai for vermelho;
    while(pai && pai->cor == 'V'){
        pai = aux->pai;
        avo = pai->pai;

        // Caso o nó esteja a ESQUERDA do avô;
        if(pai == avo->esq){
            tio = avo->dir;

            // Caso 1: Tio é VERMELHO;
            if(tio && tio->cor == 'V'){
                pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                aux = avo; // Nova avaliação para o avô;
            }

            // Casos 2 e 3: Tio é PRETO;
            else{
                // Caso 2: Nó é filho da DIREITA;
                if(aux == pai->dir){ 
                    aux = pai;

                    rotacaoEsquerda(aux);
                    // Vira Caso 3;
                }

                // Caso 3: Nó é filho da ESQUERDA;
                pai->cor = 'P';
                avo->cor = 'V';

                rotacaoDireita(avo);
            }
        }

        // Caso o nó esteja a DIREITA do avô;
        else{
            tio = avo->esq;

            // Caso 1: Tio é VERMELHO;
            if(tio && tio->cor == 'V'){
                pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                aux = avo; // Nova avaliação para o avô;
            }

            // Casos 2 e 3: Tio é PRETO;
            else{
                // Caso 2: Nó é filho da ESQUERDA;
                if(aux == pai->esq){
                    aux = pai;

                    rotacaoDireita(aux);
                    // Vira Caso 3;
                }

                // Caso 3: Nó é filho da DIREITA;
                pai->cor = 'P';
                avo->cor = 'V';

                rotacaoEsquerda(avo);
            }
        }
    }

    arv->sentinela->dir->cor = 'P'; // Raiz colorida de preto;
}

void balanceamentoRemocao(arvoreRB* arv, noRB* noSucessor, noRB* noPai){
    noRB *aux = noSucessor;
    noRB *pai = noPai;
    noRB *irmao = NULL;
    
    // Avalia o nó enquanto não for a raiz e for preto;
    while(aux != arv->sentinela->dir && (!aux || aux->cor == 'P')){
        // Caso o nó for filho da ESQUERDA;
        if(aux == pai->esq){
            irmao = pai->dir;

            // Caso 1: Irmão é VERMELHO;
            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                pai->cor = 'V';

                rotacaoEsquerda(pai);

                irmao = pai->dir;
            }

            // Caso 2: Ambos os filhos do irmão são PRETOS;
            if((!irmao->esq || irmao->esq->cor == 'P') && (!irmao->dir || irmao->dir->cor == 'P')){
                irmao->cor = 'V';
                aux = pai; // Reavalia para o pai;
            }

            // Casos 3 e 4: Filho ESQUERDO do irmão é VERMELHO;
            else{ 
                // Caso 3: Filho DIREITO do irmão é PRETO;
                if(!irmao->dir || irmao->dir->cor == 'P'){
                    irmao->esq->cor = 'P';
                    irmao->cor = 'V';

                    rotacaoDireita(irmao);

                    irmao = pai->dir;
                    // Vira o Caso 4;
                }

                // Caso 4: Ambos os filhos do irmão são VERMELHOS;
                irmao->cor = pai->cor;
                pai->cor = 'P';
                irmao->dir->cor = 'P';

                rotacaoEsquerda(pai);

                aux = arv->sentinela->dir; // Força o encerramento do balanceamento;
            }
        }

        // Caso o nó for filho da DIREITA;
        else{
            irmao = pai->esq;

            // Caso 1: Irmão é VERMELHO;
            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                pai->cor = 'V';

                rotacaoDireita(pai);

                irmao = pai->esq;
            }

            // Caso 2: Ambos os filhos do irmão são PRETOS;
            if((!irmao->esq || irmao->esq->cor == 'P') && (!irmao->dir || irmao->dir->cor == 'P')){
                irmao->cor = 'V';
                aux = pai; // Reavalia para o pai;
            }

            // Casos 3 e 4: Filho DIREITO do irmão é VERMELHO;
            else{ 
                // Caso 3: Filho ESQUERDO do irmão é PRETO;
                if(!irmao->esq || irmao->esq->cor == 'P'){
                    irmao->dir->cor = 'P';
                    irmao->cor = 'V';

                    rotacaoEsquerda(irmao);

                    irmao = pai->esq;
                }

                // Caso 4: Ambos os filhos do irmão são VERMELHOS;
                irmao->cor = pai->cor;
                pai->cor = 'P';
                irmao->esq->cor = 'P';

                rotacaoDireita(pai);

                aux = arv->sentinela->dir; // Força o encerramento do balanceamento;
            }
        }
    }

    aux->cor = 'P';
}

void rotacaoEsquerda(noRB* noDesbalanceado){
    noRB* pai = noDesbalanceado->pai;
    noRB* filho = noDesbalanceado->dir; // Nó que se tornará pai;
    noRB* neto = filho->esq;

    noDesbalanceado->dir = neto; // Remove a ligação do nó com o filho;

    // Caso o neto exista, seu avô se torna seu pai;
    if(neto) neto->pai = noDesbalanceado;

    // Atualização do pai do filho;
    if(noDesbalanceado == pai->esq)
        pai->esq = filho;
    else
        pai->dir = filho;

    filho->pai = pai;

    // Filho se torna pai de seu pai;
    filho->esq = noDesbalanceado; 
    noDesbalanceado->pai = filho;
}

void rotacaoDireita(noRB* noDesbalanceado){
    noRB* pai = noDesbalanceado->pai;
    noRB* filho = noDesbalanceado->esq; // Nó que se tornará pai;
    noRB* neto = filho->dir;

    noDesbalanceado->esq = neto; // Remove a ligação do nó com o filho;

    // Caso o neto exista, seu avô se torna seu pai;
    if(neto) neto->pai = noDesbalanceado;

    // Atualização do pai do filho;
    if(noDesbalanceado == pai->esq)
        pai->esq = filho;
    else
        pai->dir = filho;

    filho->pai = pai;

    // Filho se torna pai de seu pai;
    filho->dir = noDesbalanceado;
    noDesbalanceado->pai = filho;
}

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////

void setRaiz(arvoreRB* arv, noRB* noRaiz){
    arv->sentinela->dir = noRaiz;
}

noRB* retornaRaizRB(arvoreRB* arv){
    return arv->sentinela->dir;
}

void imprimePreOrdemRB(arvoreRB* arv, noRB* aux){
    if(!aux) return; // Condição de parada;

    printf("[%d | %c]\n", aux->chave, aux->cor);
    imprimePreOrdemRB(arv, aux->esq);
    imprimePreOrdemRB(arv, aux->dir);
}

void imprimeArvoreRB(arvoreRB* arv){
    if (arv->sentinela->dir == NULL){
        printf("A árvore está vazia.\n");
        return;
    }

    imprimeNoRB(arv->sentinela->dir, "", 1);
}

void imprimeNoRB(noRB* no, const char* prefixo, int ultimoNo){
    if(!no) return;

    printf("%s", prefixo);

    // Printa as arestas da árvore;
    if(ultimoNo == 1)
        printf("└── ");
    else
        printf("├── ");

    printf("[%d, %c]\n", no->chave, no->cor); // Printa o nó (chave e cor);

    char novoPrefixo[1024];
    if(ultimoNo)
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s    ", prefixo);
    else
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s│   ", prefixo);

    // Verifica se o nó tem filhos;
    int filhosEsq = 0;
    int filhosDir = 0;

    if(no->esq) filhosEsq = 1;
    if(no->dir) filhosDir = 1;

    if(filhosEsq || filhosDir){
        imprimeNoRB(no->esq, novoPrefixo, !filhosDir);
        imprimeNoRB(no->dir, novoPrefixo, 1);
    }
}
