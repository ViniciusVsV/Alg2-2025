#include "btree_padrao.h"

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
};

struct no
{
    int qnt_chaves;
    int chaves[MAXIMO_CHAVES + 1];  // +1 para permitir overflow temporário
    no *filhos[MAXIMO_FILHOS + 1];  // +1 para permitir overflow temporário
    
    int folha;
};

btree* alocaBtree(){
    btree* novaArvore = (btree*)calloc(1, sizeof(btree));
    if(!novaArvore)
        return NULL;

    novaArvore->qntSplit = 0;
    novaArvore->qntMerge = 0;
    novaArvore->qntRotacoes = 0;
    novaArvore->raiz = NULL;

    return novaArvore;
}

no* alocaNo(int folha){
    no* novoNo = calloc(1, sizeof(no));
    if(!novoNo)
        return NULL;

    novoNo->folha = folha;
    novoNo->qnt_chaves = 0;

    // Inicializa as chaves com -1
    for (int i = 0; i < MAXIMO_CHAVES + 1; i++){
        novoNo->chaves[i] = -1;
    }
    
    // Inicializa os ponteiros para filhos como NULL
    for (int i = 0; i < MAXIMO_FILHOS + 1; i++){
        novoNo->filhos[i] = NULL;
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
    // Verificar se a árvore está vazia
    if(arv->raiz == NULL){
        arv->raiz = alocaNo(1);
        arv->raiz->chaves[0] = chave;
        arv->raiz->qnt_chaves = 1;
        return;
    }

    // Inserir na árvore usando abordagem não-preemptiva
    insereNoBtree(arv, arv->raiz, chave);
}

void insereNoBtree(btree *arv, no* no_atual, int chave) {
    if (no_atual->folha) {
        // Se é folha, insere a chave
        insereChaveNoBtree(no_atual, chave);
        // E depois repara a árvore se o nó ficou cheio demais
        repararInsercao(arv, no_atual);
    } else {
        // Se não é folha, encontrar o filho correto para descer
        int i = 0;
        while (i < no_atual->qnt_chaves && chave > no_atual->chaves[i]) {
            i++;
        }
        // Descer para o filho encontrado
        insereNoBtree(arv, no_atual->filhos[i], chave);
    }
}

void repararInsercao(btree *arv, no *no_atual) {
    // Se o nó não está cheio demais, não precisa reparar  
    if (no_atual->qnt_chaves <= MAXIMO_CHAVES) {
        return;
    }
    
    // Se é a raiz, dividir e criar nova raiz
    if (no_atual == arv->raiz) {
        arv->raiz = dividirNoBtree(arv, no_atual, NULL, -1);
        return;
    }
    
    // Encontrar o pai e dividir o nó
    no *pai = encontrarPai(arv->raiz, no_atual);
    if (pai != NULL) {
        int parentIndex = 0;
        while (parentIndex <= pai->qnt_chaves && pai->filhos[parentIndex] != no_atual) {
            parentIndex++;
        }
        
        dividirNoBtree(arv, no_atual, pai, parentIndex);
        
        // Reparar recursivamente o pai se necessário
        repararInsercao(arv, pai);
    }
}

no* dividirNoBtree(btree *arv, no *no_cheio, no *pai, int parentIndex) {
    int split_index = MAXIMO_CHAVES / 2;  // Para ordem 4: split_index = 1
    int chaveSubindo = no_cheio->chaves[split_index];
    
    // Criar novo nó direito
    no *novoNo = alocaNo(no_cheio->folha);
    if (!novoNo) return pai;
    
    // Transferir chaves à direita da chave que sobe para o novo nó
    novoNo->qnt_chaves = no_cheio->qnt_chaves - split_index - 1;
    for (int i = 0; i < novoNo->qnt_chaves; i++) {
        novoNo->chaves[i] = no_cheio->chaves[split_index + 1 + i];
    }
    
    // Se não é folha, transferir filhos também
    if (!no_cheio->folha) {
        for (int i = 0; i <= novoNo->qnt_chaves; i++) {
            novoNo->filhos[i] = no_cheio->filhos[split_index + 1 + i];
            no_cheio->filhos[split_index + 1 + i] = NULL;
        }
    }
    
    // Atualizar número de chaves no nó original (lado esquerdo)
    no_cheio->qnt_chaves = split_index;
    
    arv->qntSplit++;
    
    // Se não tem pai, criar nova raiz
    if (pai == NULL) {
        no *novaRaiz = alocaNo(0);
        if (!novaRaiz) return no_cheio;
        
        novaRaiz->chaves[0] = chaveSubindo;
        novaRaiz->qnt_chaves = 1;
        novaRaiz->filhos[0] = no_cheio;
        novaRaiz->filhos[1] = novoNo;
        
        return novaRaiz;
    }
    
    // Inserir chave no pai
    // Primeiro, deslocar ponteiros para filhos para a direita
    for (int i = pai->qnt_chaves; i >= parentIndex + 1; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }
    
    // Depois, deslocar chaves para a direita
    for (int i = pai->qnt_chaves - 1; i >= parentIndex; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }
    
    // Inserir a chave que subiu e o novo nó filho
    pai->chaves[parentIndex] = chaveSubindo;
    pai->filhos[parentIndex + 1] = novoNo;
    pai->qnt_chaves++;
    
    return pai;
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

    removeNoBtree(arv, arv->raiz, chave);
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
            // Descer para o último filho
            removeNoBtree(arv, no_atual->filhos[no_atual->qnt_chaves], chave);
        }
        // Se for folha, a chave não existe
        return;
    }
    
    // Chave é maior que a chave atual
    else if (no_atual->chaves[i] > chave) {
        if (!no_atual->folha) {
            // Descer para o filho apropriado
            removeNoBtree(arv, no_atual->filhos[i], chave);
        }
        // Se for folha, a chave não existe
        return;
    }
    
    // A chave foi encontrada
    else {
        if (no_atual->folha) {
            // Remove a chave e repara depois
            removeChaveNoBtree(no_atual, chave);
            repararRemocao(arv, no_atual);
        } else {
            // Encontrar predecessor
            no *maxNode = no_atual->filhos[i];
            while (!maxNode->folha) {
                maxNode = maxNode->filhos[maxNode->qnt_chaves];
            }
            
            // Substituir a chave pelo predecessor
            no_atual->chaves[i] = maxNode->chaves[maxNode->qnt_chaves - 1];
            
            // Remover o predecessor da folha
            maxNode->qnt_chaves--;
            
            // Reparar a árvore após a remoção
            repararRemocao(arv, maxNode);
        }
    }
}

void repararRemocao(btree *arv, no *no_atual) {
    // Se o nó tem chaves suficientes, não precisa reparar
    if (no_atual->qnt_chaves >= MINIMO_CHAVES) {
        return;
    }
    
    // Se é a raiz
    if (no_atual == arv->raiz) {
        if (no_atual->qnt_chaves == 0) {
            // Raiz vazia
            no *raizAntiga = arv->raiz;
            if (!arv->raiz->folha) {
                arv->raiz = arv->raiz->filhos[0];
            } else {
                arv->raiz = NULL;
            }
            free(raizAntiga);
        }
        return;
    }
    
    // Encontrar o pai e o índice do nó atual
    no *pai = encontrarPai(arv->raiz, no_atual);
    if (pai == NULL) return;
    
    int parentIndex = 0;
    while (parentIndex <= pai->qnt_chaves && pai->filhos[parentIndex] != no_atual) {
        parentIndex++;
    }
    
    // Tentar emprestar do irmão da esquerda
    if (parentIndex > 0 && pai->filhos[parentIndex - 1]->qnt_chaves > MINIMO_CHAVES) {
        emprestaEsquerda(arv, pai, parentIndex);
    }
    // Tentar emprestar do irmão da direita
    else if (parentIndex < pai->qnt_chaves && pai->filhos[parentIndex + 1]->qnt_chaves > MINIMO_CHAVES) {
        emprestaDireita(arv, pai, parentIndex);
    }
    // Juntar com um irmão
    else {
        no *mergedNode;
        if (parentIndex == 0) {
            // Juntar com irmão da direita
            mergedNode = juntarNoBtree(arv, pai, parentIndex);
        } else {
            // Juntar com irmão da esquerda
            mergedNode = juntarNoBtree(arv, pai, parentIndex - 1);
        }
        
        // Reparar recursivamente o pai
        repararRemocao(arv, pai);
    }
}

no* encontrarPai(no *raiz, no *filho) {
    if (raiz == NULL || raiz->folha) {
        return NULL;
    }
    
    // Verificar se algum dos filhos é o nó procurado
    for (int i = 0; i <= raiz->qnt_chaves; i++) {
        if (raiz->filhos[i] == filho) {
            return raiz;
        }
    }
    
    // Buscar recursivamente nos filhos
    for (int i = 0; i <= raiz->qnt_chaves; i++) {
        no *pai = encontrarPai(raiz->filhos[i], filho);
        if (pai != NULL) {
            return pai;
        }
    }
    
    return NULL;
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