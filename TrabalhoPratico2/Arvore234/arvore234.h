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

//Insere um novo elemento no nó de forma ordenada
void insereChaveNo(no* no, int chave);

//Realize a operação de split em um nó folha cheio
void dividirNo();

#endif