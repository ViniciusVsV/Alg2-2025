#include "btree.h"

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
        // printf("Chave %d não encontrada.\n", chave);
        return;
    }

    // Deslocar as chaves para a esquerda
    for (int i = index; i < no->qnt_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
    }

    no->qnt_chaves--;
}

void insereBtree(btree* arv, int chave){
    // printf("Inserindo chave: %d\n", chave);
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
    int index = aux->qnt_chaves - 1, last_index = 0; // Índice do último elemento no nó

    // Encontra o nó folha onde a chave deve ser inserida
    while (aux->folha == 0) {
        //Encontrar o filho correto para descer
        index = aux->qnt_chaves - 1; // Atualiza o índice para o novo nó
        
        while (index >= 0 && aux->chaves[index] >= chave) {
            index--;
        }

        if (aux->qnt_chaves == MAXIMO_CHAVES && pai) {
            // printf("Nó interno cheio, dividindo...\n");
            // Se o nó interno está cheio, dividir o nó
            dividirNo(arv, pai, last_index);

            // Após a divisão, verificar novamente onde inserir a chave
            if (chave > pai->chaves[last_index-1]) {
                aux = pai->filhos[last_index]; // Desce para o filho à direita
            }
        }

        index++; // Corrigir o índice para apontar para o filho correto

        pai = aux; // Guarda o nó pai
        aux = aux->filhos[index];
        last_index = index; // Atualiza o índice do último filho visitado
    }

    // Agora 'aux' é um nó folha onde a chave deve ser inserida
    if (aux->qnt_chaves == MAXIMO_CHAVES) {
        // printf("Nó folha cheio, dividindo...\n");
        // Se o nó folha está cheio, dividir o nó
        dividirNo(arv, pai, index);

        // Após a divisão, verificar novamente onde inserir a chave
        if (chave > pai->chaves[index]) {
            aux = pai->filhos[index+1]; // Desce para o filho à direita
        }
    }

    // Inserir a chave no nó folha
    insereChaveNoBtree(aux, chave);
}

void dividirNo(btree *arv, no* pai, int index){
    // printf("Dividindo nó no índice %d\n", index);
    no* filho = pai->filhos[index];
    no* novoNo = alocaNo(filho->folha);
    if(!novoNo)
        return;

    // Transferir metade das chaves do filho para o novo nó
    int meio = MAXIMO_FILHOS / 2 - 1; // Índice do meio
    novoNo->qnt_chaves = meio;
    // printf("Meio: %d\n", meio);

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

    arv->qntSplit++; // Incrementa o contador de splits
}

void juntarNoBtree(no *pai, int index) {
    no *filho = pai->filhos[index], *irmao = pai->filhos[index + 1];

    // Chave do pai desce e se torna a chave do meio no filho
    filho->chaves[MINIMO_CHAVES] = pai->chaves[index];

    // Copia chaves do irmão para o final do filho
    for (int i = 0; i < irmao->qnt_chaves; i++) {
        filho->chaves[i + MINIMO_CHAVES + 1] = irmao->chaves[i];
    }

    // Copia filhos do irmão
    if (!filho->folha) {
        for (int i = 0; i <= irmao->qnt_chaves; i++) {
            filho->filhos[i + MINIMO_CHAVES + 1] = irmao->filhos[i];
        }
    }

    // Move chaves e filhos do pai para preencher o buraco
    for (int i = index + 2; i <= pai->qnt_chaves; i++) {
        pai->chaves[i - 2] = pai->chaves[i - 1];
        pai->filhos[i - 1] = pai->filhos[i];
    }

    pai->filhos[pai->qnt_chaves] = NULL; // Limpa o último filho

    // Atualiza o número de chaves no filho e no pai
    filho->qnt_chaves += irmao->qnt_chaves + 1;
    pai->qnt_chaves--;

    free(irmao); // Libera o nó irmão
}

void preencherNoBtree(no *pai, int index) {
    // printf("Preenchendo nó no índice %d\n", index);
    // Verifica se o nó filho da esquerda tem chaves suficientes
    if (index != 0 && pai->filhos[index-1]->qnt_chaves > MINIMO_CHAVES) {
        // printf("Preenchimento: Filho à esquerda tem chaves suficientes.\n");
        no *filho = pai->filhos[index], *filhoEsquerdo = pai->filhos[index - 1];

        // Move chaves no filho para abrir espaço
        for (int i = filho->qnt_chaves - 1; i >= 0; --i) {
            filho->chaves[i + 1] = filho->chaves[i];
        }

        // Se não for folha, move filhos
        if (!filho->folha) {
            for (int i = filho->qnt_chaves; i >= 0; --i) {
                filho->filhos[i + 1] = filho->filhos[i];
            }
        }

        // Se não for folha, o último filho do irmão passa para o filho
        if (!filho->folha) {
            filho->filhos[0] = filhoEsquerdo->filhos[filhoEsquerdo->qnt_chaves];
        }

       // Chave do irmão sobe para o pai
        pai->chaves[index - 1] = filhoEsquerdo->chaves[filhoEsquerdo->qnt_chaves - 1];

        filho->qnt_chaves++;
        filhoEsquerdo->qnt_chaves--;

        // Verifica se o nó filho da direita tem chaves suficientes
    } else if (index != pai->qnt_chaves && pai->filhos[index+1]->qnt_chaves > MINIMO_CHAVES) {
        // printf("Preenchimento: Filho à direita tem chaves suficientes.\n");
        no *filho = pai->filhos[index], *filhoDireito = pai->filhos[index+1];

        // Chave do pai desce para o final do filho
        filho->chaves[filho->qnt_chaves] = pai->chaves[index];

        // Primeiro filho do irmão direito se torna o último filho do filho
        if (!filho->folha) {
            filho->filhos[filho->qnt_chaves + 1] = filhoDireito->filhos[0];
        }

        // Chave do irmão sobe para o pai
        pai->chaves[index] = filhoDireito->chaves[0];

        // Move chaves e filhos do irmão para a esquerda
        for (int i = 1; i < filhoDireito->qnt_chaves; i++) {
            filhoDireito->chaves[i - 1] = filhoDireito->chaves[i];
        }
        if (!filhoDireito->folha) {
            for (int i = 1; i <= filhoDireito->qnt_chaves; i++) {
                filhoDireito->filhos[i - 1] = filhoDireito->filhos[i];
            }
        }

        filho->qnt_chaves++;
        filhoDireito->qnt_chaves--;
    } else {
        // printf("Preenchimento: Nenhum filho tem chaves suficientes. Juntando nós.\n");
        // Se nenhum dos filhos tem chaves suficientes, fundir os nós
        if (index == pai->qnt_chaves) {
            index--; // Se o índice for igual ao número de chaves, ajusta para o filho à esquerda
        }

        juntarNoBtree(pai, index);
    }
}

void removeBtree(btree *arv, int chave) {
    if (arv->raiz == NULL) {
        // printf("A árvore está vazia.\n");
        return;
    }

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
    // printf("Removendo chave: %d\n", chave);
    no *aux = no_atual, *pai = NULL;
    int index = 0;

    // Encontrar o nó onde a chave está localizada
    while (aux != NULL) {
        index = 0;
        while (index < aux->qnt_chaves && aux->chaves[index] < chave) {
            index++;
        }

        if (index < aux->qnt_chaves && aux->chaves[index] == chave) {
            if(aux->folha) {
                removeChaveNoBtree(aux, chave);
            } else {
                int chave = aux->chaves[index];

                if(aux->filhos[index]->qnt_chaves > MINIMO_CHAVES) {
                    // Se o filho à esquerda tem chaves suficientes, pegar o predecessor
                    // printf("Filho à esquerda tem chaves suficientes.\n");
                    no *noPredecessor = aux->filhos[index], *paiPredecessor = aux;
                    while (!noPredecessor->folha) {
                        paiPredecessor = noPredecessor;
                        noPredecessor = noPredecessor->filhos[noPredecessor->qnt_chaves];
                    }
                    aux->chaves[index] = noPredecessor->chaves[noPredecessor->qnt_chaves - 1];
                    removeChaveNoBtree(noPredecessor, aux->chaves[index]);

                    if(noPredecessor->qnt_chaves < MINIMO_CHAVES) {
                        preencherNoBtree(paiPredecessor, index);
                        arv->qntRotacoes++;
                    }

                } else if (aux->filhos[index + 1]->qnt_chaves > MINIMO_CHAVES) {
                    // Se o filho à direita tem chaves suficientes, pegar o sucessor
                    // printf("Filho à direita tem chaves suficientes.\n");
                    no *noSucessor = aux->filhos[index + 1], *paiSucessor = aux;
                    while (!noSucessor->folha) {
                        paiSucessor = noSucessor;
                        noSucessor = noSucessor->filhos[0];
                    }
                    aux->chaves[index] = noSucessor->chaves[0];
                    
                    // Verifica se o nó sucessor tem chaves suficientes
                    if(noSucessor->qnt_chaves == MINIMO_CHAVES) {
                        preencherNoBtree(paiSucessor, index);
                        arv->qntRotacoes++;
                    }

                    removeChaveNoBtree(noSucessor, aux->chaves[index]);

                } else {
                    // printf("Nenhum filho tem chaves suficientes. Juntando nós.\n");
                    // Se ambos os filhos não têm chaves suficientes, juntar os nós
                    juntarNoBtree(aux, index);
                    removeChaveNoBtree(aux->filhos[index], chave);
                }
            }
            aux = NULL;
        } else {
            if (aux->folha) {
                // Se for uma folha e a chave não foi encontrada, sair
                printf("Chave %d não encontrada.\n", chave);
                return;
            }

            int flag = index == aux->qnt_chaves;

            if (aux->filhos[index]->qnt_chaves == MINIMO_CHAVES) {
                preencherNoBtree(aux, index);
                arv->qntRotacoes++;
            }

            if(flag && index > aux->qnt_chaves) {
                index--; // Ajusta o índice se for o último filho
            }

            pai = aux; // Guarda o nó pai
            aux = aux->filhos[index]; // Desce para o filho correto
        }
    }

}

// Getter functions for statistics
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