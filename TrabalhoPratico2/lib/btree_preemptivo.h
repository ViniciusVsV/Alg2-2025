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

// Insere um novo elemento na árvore btree
void insereChaveNaoCheio(btree* arv, no *no, int chave);

//Realize a operação de split em um nó folha cheio
void dividirNo(btree *arv, no* pai, int index);

// Imprime o nó atual da árvore btree
void prettyPrintNode(no *no, const char *prefix, int is_last);

// Imprime a árvore btree
void imprimeBtree(btree *arv);

// Remove um elemento da árvore btree
void removeBtree(btree *arv, int chave);

// Remove uma chave de um nó da btree
void removeNoBtree(btree *arv, no *no_atual, int chave);

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