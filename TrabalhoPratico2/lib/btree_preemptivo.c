#include "btree_preemptivo.h"

#include <stdio.h>
#include <stdlib.h>

#define ORDEM 4
#define MAXIMO_CHAVES (ORDEM - 1)
#define MAXIMO_FILHOS ORDEM
#define MINIMO_CHAVES (ORDEM / 2 - 1)

struct btree
{
    no* raiz;
    
    int qntSplit;
    int qntMerge;
    int qntRotacoes;
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

    novaArvore->qntSplit = 0;
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

void insereChaveNoBtree(no *no, int chave) {
    int index = no->qnt_chaves - 1; // Índice do último elemento no nó
    
    while (index >= 0 && no->chaves[index] > chave) {
        no->chaves[index + 1] = no->chaves[index]; // Desloca as chaves para a direita
        index--;
    }

    no->chaves[index + 1] = chave; // Insere a nova chave na posição correta

    no->qnt_chaves++;
}

void removeChaveNoBtree(no *no, int chave) {
    int index = 0;

    // Encontrar a chave a ser removida
    while (index < no->qnt_chaves && no->chaves[index] < chave) {
        index++;
    }

    // Se a chave não foi encontrada
    if (index == no->qnt_chaves || no->chaves[index] != chave) {
        printf("Chave %d não encontrada.\n", chave);
        return;
    }

    // Deslocar as chaves para a esquerda
    for (int i = index; i < no->qnt_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
    }

    no->qnt_chaves--;
}

void insereBtree(btree* arv, int chave){
    //printf("Inserindo chave: %d\n", chave);
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
    insereChaveNaoCheio(arv, arv->raiz, chave);
}

void insereChaveNaoCheio(btree *arv, no* no, int chave){
    int index = no->qnt_chaves - 1; // Índice do último elemento no nó

    if (no->folha) {
        while(index >= 0 && no->chaves[index] > chave) {
            no->chaves[index + 1] = no->chaves[index]; // Desloca as chaves para a direita
            index--;
        }
        no->chaves[index + 1] = chave; // Insere a nova chave na posição correta
        no->qnt_chaves++;
    } else {
        while (index >= 0 && no->chaves[index] > chave) {
            index--;
        }

        index++; // Corrige o índice para apontar para o filho correto

        if(no->filhos[index]->qnt_chaves == MAXIMO_CHAVES) {
            // Se o filho está cheio, dividir o nó
            dividirNo(arv, no, index);
            if (no->chaves[index] < chave) {
                index++; // Ajusta o índice se a chave for maior que a chave do pai
            }
        }

        insereChaveNaoCheio(arv, no->filhos[index], chave); // Desce para o filho correto
    }
}

void dividirNo(btree *arv, no* pai, int index){
    //printf("Dividindo nó no índice %d\n", index);
    no* filho = pai->filhos[index];
    no* novoNo = alocaNo(filho->folha);
    if(!novoNo)
        return;

    // Transferir metade das chaves do filho para o novo nó
    int meio = MAXIMO_FILHOS / 2 - 1; // Índice do meio
    novoNo->qnt_chaves = meio;

    for (int i = 0; i < meio; i++) {
        novoNo->chaves[i] = filho->chaves[i + meio + 1]; // +1 para pular a chave do meio
    }

    // Se o filho não é uma folha, transferir os filhos também
    if (!filho->folha) {
        for (int i = 0; i <= meio; i++) {
            novoNo->filhos[i] = filho->filhos[i + meio + 1];
        }
    }

    // Atualizar o número de chaves no filho
    filho->qnt_chaves = meio;

    // Inserir a nova chave no pai
    for (int i = pai->qnt_chaves; i > index; i--) {
        pai->chaves[i] = pai->chaves[i - 1]; // Desloca as chaves para a direita
        pai->filhos[i + 1] = pai->filhos[i]; // Desloca os filhos para a direita
    }

    pai->chaves[index] = filho->chaves[meio]; // A chave do meio sobe para o pai
    pai->filhos[index + 1] = novoNo; // O novo nó é adicionado como filho

    pai->qnt_chaves++; // Incrementa o número de chaves no pai

    arv->qntSplit++; // Incrementa o contador de splits
}

no* juntarNoBtree(btree *arv, no *pai, int index) {
    no *filho = pai->filhos[index], *irmao = pai->filhos[index + 1];

    // Chave do pai desce e se torna a nova chave no filho
    filho->chaves[filho->qnt_chaves] = pai->chaves[index];

    // Copia chaves do irmão para o final do filho
    for (int i = 0; i < irmao->qnt_chaves; i++) {
        filho->chaves[filho->qnt_chaves + 1 + i] = irmao->chaves[i];
    }

    // Copia filhos do irmão
    if (!filho->folha) {
        for (int i = 0; i <= irmao->qnt_chaves; i++) {
            filho->filhos[filho->qnt_chaves + 1 + i] = irmao->filhos[i];
        }
    }

    // Move chaves e filhos do pai para preencher o buraco
    for (int i = index + 1; i < pai->qnt_chaves; i++) {
        pai->chaves[i - 1] = pai->chaves[i];
        pai->filhos[i] = pai->filhos[i + 1];
    }

    // Atualiza o número de chaves no filho e no pai
    filho->qnt_chaves += irmao->qnt_chaves + 1;
    pai->qnt_chaves--;

    arv->qntMerge++; // Incrementa o contador de merges

    free(irmao); // Libera o nó irmão
    
    return filho; // Retorna o nó que contém os elementos juntados
}

void removeBtree(btree *arv, int chave) {
    if (arv->raiz == NULL) {
        printf("A árvore está vazia.\n");
        return;
    }

    // Use preemptive deletion algorithm
    removeNoBtree(arv, arv->raiz, chave);

    // Se a raiz ficou vazia após a remoção, atualizar a raiz
    if (arv->raiz->qnt_chaves == 0) {
        no *raizAntiga = arv->raiz;
        if (arv->raiz->folha) {
            arv->raiz = NULL; // Se a raiz era uma folha, agora está vazia
        } else {
            arv->raiz = arv->raiz->filhos[0]; // Caso contrário, desce para o primeiro filho
        }
        free(raizAntiga); // Libera a memória da antiga raiz
    }
}

void removeNoBtree(btree *arv, no *no_atual, int chave) {
    if (no_atual == NULL) {
        return;
    }

    int i = 0;
    
    // Encontrar a posição da chave
    while (i < no_atual->qnt_chaves && no_atual->chaves[i] < chave) {
        i++;
    }

    // Filho mais à direita
    if (i == no_atual->qnt_chaves) {
        if (!no_atual->folha) {
            // Verificar se o último filho tem chaves suficientes
            if (no_atual->filhos[no_atual->qnt_chaves]->qnt_chaves == MINIMO_CHAVES) {
                no *nextNode;

                if (no_atual->filhos[no_atual->qnt_chaves - 1]->qnt_chaves > MINIMO_CHAVES) {
                    // Emprestar do irmão da esquerda
                    nextNode = emprestaEsquerda(arv, no_atual, no_atual->qnt_chaves);
                    removeNoBtree(arv, nextNode, chave);
                } else {
                    // Juntar com o irmão da esquerda
                    nextNode = juntarNoBtree(arv, no_atual, no_atual->qnt_chaves - 1);
                    removeNoBtree(arv, nextNode, chave);
                }
                
            } else {
                // O filho tem chaves suficientes
                removeNoBtree(arv, no_atual->filhos[no_atual->qnt_chaves], chave);
            }
        }
        // Se for folha, a chave não existe
        return;
    }
    
    // Chave é maior que a chave atual
    else if (no_atual->chaves[i] > chave) {
        if (!no_atual->folha) {
            // Verificar se o filho tem chaves suficientes
            if (no_atual->filhos[i]->qnt_chaves > MINIMO_CHAVES) {
                removeNoBtree(arv, no_atual->filhos[i], chave);
            } else {
                no *nextNode;
                if (no_atual->filhos[i + 1]->qnt_chaves > MINIMO_CHAVES) {
                    // Emprestar do irmão da direita
                    nextNode = emprestaDireita(arv, no_atual, i);
                    removeNoBtree(arv, nextNode, chave);
                } else {
                    // Juntar com o irmão da direita
                    nextNode = juntarNoBtree(arv, no_atual, i);
                    removeNoBtree(arv, nextNode, chave);
                }
            }
        }
        // Se for folha, a chave não existe - não faz nada
        return;
    }
    
    // A chave foi encontrada
    else {
        if (no_atual->folha) {
            // Caso simples: nó folha - remove a chave
            removeChaveNoBtree(no_atual, chave);
        } else {
            // Nó interno - verificar os filhos adjacentes
            if (no_atual->filhos[i]->qnt_chaves == MINIMO_CHAVES) {
                // Filho à esquerda tem apenas o mínimo
                if (no_atual->filhos[i + 1]->qnt_chaves == MINIMO_CHAVES) {
                    // Ambos os filhos têm o mínimo - juntar e deletar recursivamente
                    no *nextNode = juntarNoBtree(arv, no_atual, i);
                    removeNoBtree(arv, nextNode, chave);
                    return;
                } else {
                    // Filho à direita tem chaves extras - usar sucessor
                    no *minNode = no_atual->filhos[i + 1];
                    while (!minNode->folha) {
                        if (minNode->filhos[0]->qnt_chaves == MINIMO_CHAVES) {
                            if (minNode->filhos[1]->qnt_chaves == MINIMO_CHAVES) {
                                minNode = juntarNoBtree(arv, minNode, 0);
                            } else {
                                minNode = emprestaDireita(arv, minNode, 0);
                            }
                        } else {
                            minNode = minNode->filhos[0];
                        }
                    }
                    
                    // Substituir a chave pelo sucessor e remover o sucessor
                    no_atual->chaves[i] = minNode->chaves[0];
                    removeChaveNoBtree(minNode, minNode->chaves[0]);
                }
            } else {
                // Filho à esquerda tem chaves extras - usar predecessor
                no *maxNode = no_atual->filhos[i];
                while (!maxNode->folha) {
                    if (maxNode->filhos[maxNode->qnt_chaves]->qnt_chaves == MINIMO_CHAVES) {
                        if (maxNode->filhos[maxNode->qnt_chaves - 1]->qnt_chaves > MINIMO_CHAVES) {
                            maxNode = emprestaEsquerda(arv, maxNode, maxNode->qnt_chaves);
                        } else {
                            maxNode = juntarNoBtree(arv, maxNode, maxNode->qnt_chaves - 1);
                        }
                    } else {
                        maxNode = maxNode->filhos[maxNode->qnt_chaves];
                    }
                }
                
                // Substituir a chave pelo predecessor e remover o predecessor
                no_atual->chaves[i] = maxNode->chaves[maxNode->qnt_chaves - 1];
                removeChaveNoBtree(maxNode, maxNode->chaves[maxNode->qnt_chaves - 1]);
            }
        }
    }
}

no* emprestaEsquerda(btree *arv, no *pai, int index) {
    no *filho = pai->filhos[index];
    no *irmaoEsquerdo = pai->filhos[index - 1];

    // Mover todas as chaves do filho uma posição para a direita
    for (int i = filho->qnt_chaves - 1; i >= 0; i--) {
        filho->chaves[i + 1] = filho->chaves[i];
    }

    // Se não for folha, mover os filhos também
    if (!filho->folha) {
        for (int i = filho->qnt_chaves; i >= 0; i--) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
        // O último filho do irmão esquerdo se torna o primeiro filho do filho
        filho->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->qnt_chaves];
    }

    // A chave do pai desce para o filho
    filho->chaves[0] = pai->chaves[index - 1];

    // A última chave do irmão esquerdo sobe para o pai
    pai->chaves[index - 1] = irmaoEsquerdo->chaves[irmaoEsquerdo->qnt_chaves - 1];

    // Atualizar contadores
    filho->qnt_chaves++;
    irmaoEsquerdo->qnt_chaves--;
    arv->qntRotacoes++;
    
    return filho;
}

no* emprestaDireita(btree *arv, no *pai, int index) {
    no *filho = pai->filhos[index];
    no *irmaoDireito = pai->filhos[index + 1];

    // A chave do pai desce para o final do filho
    filho->chaves[filho->qnt_chaves] = pai->chaves[index];

    // Se não for folha, o primeiro filho do irmão direito se torna o último filho do filho
    if (!filho->folha) {
        filho->filhos[filho->qnt_chaves + 1] = irmaoDireito->filhos[0];
    }

    // A primeira chave do irmão direito sobe para o pai
    pai->chaves[index] = irmaoDireito->chaves[0];

    // Mover todas as chaves do irmão direito uma posição para a esquerda
    for (int i = 1; i < irmaoDireito->qnt_chaves; i++) {
        irmaoDireito->chaves[i - 1] = irmaoDireito->chaves[i];
    }

    // Se não for folha, mover os filhos também
    if (!irmaoDireito->folha) {
        for (int i = 1; i <= irmaoDireito->qnt_chaves; i++) {
            irmaoDireito->filhos[i - 1] = irmaoDireito->filhos[i];
        }
    }

    // Atualizar contadores
    filho->qnt_chaves++;
    irmaoDireito->qnt_chaves--;
    arv->qntRotacoes++;
    
    return filho;
}

int obterQntSplit(btree *arv) {
    if (!arv) return 0;
    return arv->qntSplit;
}

int obterQntMerge(btree *arv) {
    if (!arv) return 0;
    return arv->qntMerge;
}

int obterQntRotacoes(btree *arv) {
    if (!arv) return 0;
    return arv->qntRotacoes;
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

int calcularAlturaBtree(btree *arv) {
    if (arv->raiz == NULL) {
        return 0; // A árvore está vazia
    }

    no *raiz = arv->raiz;
    int altura = 1; // Começa contando a raiz

    while (!raiz->folha) {
        altura++;
        raiz = raiz->filhos[0]; // Desce para o primeiro filho
    }

    return altura;
}

int contarNosRecursivo(no* no_atual) {
    if (no_atual == NULL) {
        return 0;
    }

    int contagem = 1; // Conta o nó atual

    // Se não for folha, conta recursivamente os nós em cada subárvore filha
    if (!no_atual->folha) {
        for (int i = 0; i <= no_atual->qnt_chaves; i++) {
            contagem += contarNosRecursivo(no_atual->filhos[i]);
        }
    }

    return contagem;
}

int contarNosBtree(btree* arv) {
    if (arv == NULL || arv->raiz == NULL || arv->raiz->qnt_chaves == 0) {
        return 0;
    }
    return contarNosRecursivo(arv->raiz);
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