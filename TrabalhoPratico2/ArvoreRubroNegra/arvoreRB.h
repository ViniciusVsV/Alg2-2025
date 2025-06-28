#ifndef ARVORERB
#define ARVORERB

typedef struct  arvoreRB arvoreRB;
typedef struct noRB noRB;

//Função que aloca uma árvore RB
arvoreRB* alocaArvoreRB();

//Função que aloca um novo nó
noRB* alocaNoRB(int chave);

//Função iterativa que insere um novo nó na árvore e chama a função que ajusta o balanceamento
void insereNoRB(arvoreRB* arv, noRB* novoNo);

//Função que remove um nó da árvore RB
//retorna 1 se o nó foi removido
//retorna 0 se o elemento não foi encontrado na árvore
int removeNoRB(arvoreRB* arv, int valor);

//Função que corrige o balanceamento após a inserção
void balanceamentoInsercao(arvoreRB* arv, noRB* novoNo);

//Função que corrige o balanceamento após a remoção
void balanceamentoRemocao(arvoreRB* arv, noRB* NoSucessor, noRB* NoPai);

//Rotação à direita
void rotacaoDireita(noRB* noDesbalanceado);

//Rotação à esquerda
void rotacaoEsquerda(noRB* noDesbalanceado);

//Função que retorna a raiz da árvore
noRB* retornaRaizRB(arvoreRB* arv);

/// @brief Imprime os elementos de uma árvore rubro negra
/// @param arv Árvore em questão
void imprimeArvoreRB(arvoreRB *arv);

/// @brief Imprime as chaves de um nó de uma árvore rubro negra
/// @param no Nó em questão
/// @param prefixo
/// @param is_last 
void imprimeNoRB(noRB* no, const char* prefixo, int is_last);

#endif