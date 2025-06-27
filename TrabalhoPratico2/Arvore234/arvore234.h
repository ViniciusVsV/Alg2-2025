#ifndef ARVORE234
#define ARVORE234

typedef struct arvore234 arvore234;
typedef struct no no;

//Aloca uma nova árvore 2-3-4 de ordem 4
arvore234* alocaArvore234();

//Aloca um novo nó da árvore 2-3-4
no* alocaNo(int folha);

//Insere um novo elemento na árvore 2-3-4
void insereChaveArvore(arvore234* arv, int chave);

//Insere uma chave em um nó folha da árvore
void insereChaveNoFolha(no* noAlvo, int chave);

//Insere uma chave em um nó interno da árvore
void insereChaveNoInterno(no* noAlvo, int chave, no* filhoEsquerdo, no* filhoDireito);

//Divide um nó cheio
void divideNo(arvore234* arv, no* pai, no* noCheio);

//Percorre a árvore em ordem
void percorreArvore(no* no);

#endif