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
void insereChaveNaoCheio(btree* arv, int chave);

//Realize a operação de split em um nó folha cheio
void dividirNo(btree *arv, no* pai, int index);

void prettyPrintNode(no *no, const char *prefix, int is_last);

void imprimeBtree(btree *arv);

#endif