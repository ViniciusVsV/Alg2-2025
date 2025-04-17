#ifndef TREE_SORT
#define TREE_SORT

typedef struct no no;

typedef struct lista {
    no* pai;
    int quant;
} lista;

lista* crialista();

no* criano(int dado);

void inserirLista(lista* lista , no* novo);

int* preencherVetor(int quant);

void emOrdem(no* raiz , int* vetOrdem , int* VetIndex);

#endif
