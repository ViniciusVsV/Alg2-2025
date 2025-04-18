#ifndef TREE_SORT
#define TREE_SORT

typedef struct no no;
typedef struct arvore arvore;

arvore *criaArvore();

no *criaNo(int valor);

void inserirNo(arvore* arv, no* novo);

void travessiaMorris(arvore *arv, int *vetOrdenado);

#endif
