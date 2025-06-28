#ifndef BTREE_H
#define BTREE_H

typedef struct btree btree;

typedef struct no no;

//Aloca uma nova árvore btree
btree* alocaBtree();

//Aloca um novo nó da btree
no* alocaNo(int folha);

//Insere um novo elemento na btree
void insereBtree(btree* arv, int chave);

// Insere um elemento usando abordagem não-preemptiva
void insereNoBtree(btree* arv, no *no_atual, int chave);

// Repara a árvore após uma inserção (abordagem não-preemptiva)  
void repararInsercao(btree *arv, no *no_atual);

// Divide um nó na abordagem não-preemptiva
no* dividirNoBtree(btree *arv, no *no_cheio, no *pai, int parentIndex);

// Insere uma chave em um nó
void insereChaveNoBtree(no *no, int chave);

// Remove uma chave de um nó
void removeChaveNoBtree(no *no, int chave);

// Imprime o nó atual da árvore btree
void prettyPrintNode(no *no, const char *prefix, int is_last);

// Imprime a árvore btree
void imprimeBtree(btree *arv);

// Remove um elemento da árvore btree
void removeBtree(btree *arv, int chave);

// Remove uma chave de um nó da btree
void removeNoBtree(btree *arv, no *no_atual, int chave);

// Repara a árvore após uma remoção (abordagem não-preemptiva)
void repararRemocao(btree *arv, no *no_atual);

// Encontra o pai de um nó
no* encontrarPai(no *raiz, no *filho);

// Empresta uma chave do irmão esquerdo para o nó especificado
no* emprestaEsquerda(btree *arv, no *pai, int index);

// Empresta uma chave do irmão direito para o nó especificado
no* emprestaDireita(btree *arv, no *pai, int index);

// Junta dois nós irmãos em um único nó
no* juntarNoBtree(btree *arv, no *pai, int index);

// Conta o número total de nós na árvore
int contarNosBtree(btree* arv);

// Calcula a altura da árvore btree
int calcularAlturaBtree(btree *arv);

// Obtém a quantidade de divisões (splits) realizadas
int obterQntSplit(btree *arv);

// Obtém a quantidade de junções (merges) realizadas
int obterQntMerge(btree *arv);

// Obtém a quantidade de rotações realizadas
int obterQntRotacoes(btree *arv);

#endif