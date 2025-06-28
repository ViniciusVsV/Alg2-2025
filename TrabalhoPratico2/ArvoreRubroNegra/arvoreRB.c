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

    noRB *esq, *dir, *pai;
};

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////
arvoreRB* alocaArvoreRB(){
    arvoreRB* novaArvore = (arvoreRB*) malloc(sizeof(arvoreRB));

    noRB* sentinela = alocaNoRB(-100000, 'P');

    novaArvore->sentinela = sentinela;

    return novaArvore;
}

noRB* alocaNoRB(int chave, char cor){
    noRB* novoNo = (noRB*) malloc(sizeof(noRB));

    novoNo->chave = chave;
    novoNo->cor = cor;

    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->pai = NULL;

    return novoNo;
}

//////////////////////////Métodos de Inserção e Remoção de Elementos na Árvore////////////////////////////
void insereNoRB(arvoreRB* arv, noRB* novoNo){
    noRB* aux = arv->sentinela->dir;
    noRB* auxPai = arv->sentinela;

    while(aux != NULL){
        auxPai = aux;

        if(novoNo->chave < aux->chave)
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    if(novoNo->chave < auxPai->chave)   auxPai->esq = novoNo;
    else                                auxPai->dir = novoNo;

    novoNo->pai = auxPai;

    balanceamentoInsercao(arv, novoNo);
}

int removeNoRB(arvoreRB* arv, int valor){
    noRB* aux = arv->sentinela->dir;
    noRB *substituto, *predecessor;
    char corRemovido;

    while(aux && aux->chave != valor){
        if(valor < aux->chave) 
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    if(!aux)    return 0;

    corRemovido = aux->cor;

    if(!aux->esq && !aux->dir){
        if(aux->pai->esq == aux)
            aux->pai->esq = NULL;
        else
            aux->pai->dir = NULL;

        if(corRemovido == 'P')
            balanceamentoRemocao(arv, NULL, aux->pai);
    }

    else if(aux->esq && aux->dir){
        predecessor = aux->esq;

        while(predecessor->dir)
            predecessor = predecessor->dir;

        aux->chave = predecessor->chave;

        if(predecessor->esq){
            if(predecessor->pai->esq == predecessor)
                predecessor->pai->esq = predecessor->esq;
            else
                predecessor->pai->dir = predecessor->esq;

            predecessor->esq->pai = predecessor->pai;
        }
        else{
            if(predecessor->pai->esq == predecessor)
                predecessor->pai->esq = NULL;
            else
                predecessor->pai->dir = NULL;
        }

        if(predecessor->cor == 'P')
            balanceamentoRemocao(arv, predecessor->esq, predecessor->pai);

        aux = predecessor;
    }

    else if(aux->esq){
        if(aux->pai->esq == aux)
            aux->pai->esq = aux->esq;
        else
            aux->pai->dir = aux->esq;

        aux->esq->pai = aux->pai;
        
        if(aux->cor == 'P')
            balanceamentoRemocao(arv, aux->esq, aux->pai);
    }

    else if(aux->dir){
        if(aux->pai->esq == aux)
            aux->pai->esq = aux->dir;
        else
            aux->pai->dir = aux->dir;

        aux->dir->pai = aux->pai;

        if(aux->cor == 'P')
            balanceamentoRemocao(arv, aux->dir, aux->pai);
    }

    free(aux);

    return 1;
}

noRB* converte234(no234* raiz234){
    noRB* raizRB = NULL;

    if(!raiz234)
        return NULL;

    int qtdChaves = obtemQtdChaves(raiz234);
    int* chaves = obtemChaves(raiz234);
    no234** filhos = obtemFilhos(raiz234);

    //Nó 2 -> um nó preto
    if (qtdChaves == 1){
        raizRB = alocaNoRB(chaves[0], 'P');
        raizRB->esq = converte234(filhos[0]);
        raizRB->dir = converte234(filhos[1]);
    }

    //Nó 3 -> um nó preto com filho vermelho à direita
    else if (qtdChaves == 2){
        raizRB = alocaNoRB(chaves[0], 'P');
        noRB* vermelho = alocaNoRB(chaves[1], 'V');
        raizRB->dir = vermelho;

        raizRB->esq = converte234(filhos[0]);
        vermelho->esq = converte234(filhos[1]);
        vermelho->dir = converte234(filhos[2]);
    }

    //Nó 4 -> um nó preto com dois filhos vermelhos
    else if (qtdChaves == 3){
        raizRB = alocaNoRB(chaves[1], 'P');
        noRB* vermelhoEsq = alocaNoRB(chaves[0], 'V');
        noRB* vermelhoDir = alocaNoRB(chaves[2], 'V');

        raizRB->esq = vermelhoEsq;
        raizRB->dir = vermelhoDir;

        vermelhoEsq->esq = converte234(filhos[0]);
        vermelhoEsq->dir = converte234(filhos[1]);
        vermelhoDir->esq = converte234(filhos[2]);
        vermelhoDir->dir = converte234(filhos[3]);
    }

    return raizRB;
}

///////////////////////////////////Métodos de Balanceamento da Árvore/////////////////////////////////////
void balanceamentoInsercao(arvoreRB* arv, noRB* novoNo){
    noRB *tio, *avo;
    
    while(novoNo->pai && novoNo->pai->cor == 'V'){
        avo = novoNo->pai->pai;

        if(novoNo->pai == avo->esq){
            tio = avo->dir;

            if(tio && tio->cor == 'V'){
                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            }

            else{
                if(novoNo == novoNo->pai->dir){
                    novoNo = novoNo->pai;

                    rotacaoEsquerda(novoNo);
                    
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoDireita(avo);
            }
        }

        else{
            tio = avo->esq;

            if(tio && tio->cor == 'V'){
                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            }

            else{ 
                if(novoNo == novoNo->pai->esq){
                    novoNo = novoNo->pai;

                    rotacaoDireita(novoNo);
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoEsquerda(avo);
            }
        }
    }

    arv->sentinela->dir->cor = 'P';
}

void balanceamentoRemocao(arvoreRB* arv, noRB* noSucessor, noRB* noPai){
    noRB* irmao;
    
    while(noSucessor != arv->sentinela->dir && (!noSucessor || noSucessor->cor == 'P')){
        if(noSucessor == noPai->esq){
            irmao = noPai->dir;

            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                noPai->cor = 'V';

                rotacaoEsquerda(noPai);

                irmao = noPai->dir;
            }

            if((!irmao->esq || irmao->esq->cor == 'P') && (!irmao->esq || irmao->dir->cor == 'P')){
                irmao->cor = 'V';
                noSucessor = noPai;
            }

            else{ 
                if(!irmao->dir || irmao->dir->cor == 'P'){
                    irmao->esq->cor = 'P';
                    irmao->cor = 'V';

                    rotacaoDireita(irmao);

                    irmao = noPai->dir;
                }

                irmao->cor = noPai->cor;
                noPai->cor = 'P';
                irmao->dir->cor = 'P';

                rotacaoEsquerda(noPai);

                noSucessor = arv->sentinela->dir;
            }
        }

        else{
            irmao = noPai->esq;

            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                noPai->cor = 'V';

                rotacaoDireita(noPai);

                irmao = noPai->esq;
            }

            if((!irmao->esq || irmao->esq->cor == 'P') && (!irmao->esq || irmao->dir->cor == 'P')){
                irmao->cor = 'V';
                noSucessor = noPai;
            }

            else{ 
                if(!irmao->esq || irmao->esq->cor == 'P'){
                    irmao->dir->cor = 'P';
                    irmao->cor = 'V';

                    rotacaoEsquerda(irmao);

                    irmao = noPai->esq;
                }

                irmao->cor = noPai->cor;
                noPai->cor = 'P';
                irmao->esq->cor = 'P';

                rotacaoDireita(noPai);

                noSucessor = arv->sentinela->dir;
            }
        }
    }

    noSucessor->cor = 'P';
}

void rotacaoEsquerda(noRB* noDesbalanceado){
    noRB* pai = noDesbalanceado->pai;
    noRB* filho = noDesbalanceado->dir;
    noRB* neto = filho->esq;

    noDesbalanceado->dir = neto;

    if(neto)
        neto->pai = noDesbalanceado;

    if(noDesbalanceado == pai->esq)
        pai->esq = filho;
    else
        pai->dir = filho;

    filho->pai = pai;

    filho->esq = noDesbalanceado;
    noDesbalanceado->pai = filho;
}

void rotacaoDireita(noRB* noDesbalanceado){
    noRB* pai = noDesbalanceado->pai;
    noRB* filho = noDesbalanceado->esq;
    noRB* neto = filho->dir;

    noDesbalanceado->esq = neto;

    if(neto)    neto->pai = noDesbalanceado;

    if(noDesbalanceado == pai->esq)     pai->esq = filho;
    else                                pai->dir = filho;

    filho->pai = pai;
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

void imprimeArvoreRB(arvoreRB* arv){
    if (arv->sentinela->dir == NULL){
        printf("A árvore está vazia.\n");
        return;
    }

    imprimeNoRB(arv->sentinela->dir, "", 1);
}

void imprimeNoRB(noRB* no, const char* prefixo, int is_last){
    if(no == NULL)
        return;

    printf("%s", prefixo);
    printf(is_last ? "└── " : "├── ");
    printf("[%d, %c]\n", no->chave, no->cor); // cor: 'V' ou 'P'

    char novoPrefixo[1024];
    if(is_last)
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s    ", prefixo);
    else
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s│   ", prefixo);

    // Verifica se tem filhos diferentes de sentinela
    int temFilhosEsq = (no->esq != NULL);
    int temFilhosDir = (no->dir != NULL);

    if(temFilhosEsq || temFilhosDir){
        imprimeNoRB(no->esq, novoPrefixo, !temFilhosDir);
        imprimeNoRB(no->dir, novoPrefixo, 1);
    }
}