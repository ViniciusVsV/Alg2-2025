#ifndef ARVORERB
#define ARVORERB

#include "../Arvore234/arvore234.h"

/////////////////////////////////Declaração de Variáveis Globais e Structs////////////////////////////////

typedef struct  arvoreRB arvoreRB;
typedef struct noRB noRB;

//////////////////////////////////Métodos de Alocação da Árvore e dos Nós/////////////////////////////////

/// @brief Aloca uma nova instância do tipo arvoreRB;
/// @return Instância alocada do tipo arvoreRB;
arvoreRB* alocaArvoreRB();

/// @brief Aloca uma nova instância do tipo noRB;
/// @param chave Chave a ser atribuída ao nó;
/// @param cor Cor a ser atribuída ao nó;
/// @return Instância alocada do tipo noRB;
noRB* alocaNoRB(int chave, char cor);

//////////////////////////Métodos de Inserção e Remoção de Elementos na Árvore////////////////////////////

/// @brief Adiciona um novo nó a árvore RB;
/// @param arv Árvore RB a receber o novo nó;
/// @param novoNo Nó a ser adicionado;
void insereNoRB(arvoreRB* arv, noRB* novoNo);

/// @brief Remove um nó de uma árvore RB;
/// @param arv Árvore RB a ter o nó removido;
/// @param valor Valor do nó a ser removido;
/// @return 1 Em caso de sucesso na remoção;
/// @return 0 Em caso de o nó não existir;
int removeNoRB(arvoreRB* arv, int valor);

//////////////////////////Algoritmo de Conversão da Árvore 2-3-4 em Árvore Rubro-Negra////////////////////////////

/// @brief Converte uma árvore RB em uma árvore 2-3-4;
/// @param raiz234 Ponteiro para raiz da árvore 234;
/// @param pai Pai dos nós RB criados
/// @return Ponteiro para raiz da árvore RB;
noRB* converte234(no234* raiz234, noRB* pai);

///////////////////////////////////Métodos de Balanceamento da Árvore/////////////////////////////////////

/// @brief Ajusta o balanceamento árvore RB após uma inserção;
/// @param arv Árvore na qual ocorreu a inserção;
/// @param novoNo Ponteiro para o nó que foi inserido;
void balanceamentoInsercao(arvoreRB* arv, noRB* novoNo);

/// @brief Ajusta o balanceamento árvore RB após uma remoção;
/// @param arv Árvore na qual ocorreu a remoção;
/// @param noSucessor Ponteiro para o nó sucessor ao que foi removido;
/// @param noPai Ponteiro para o nó pai do nó que foi removido;
void balanceamentoRemocao(arvoreRB* arv, noRB* NoSucessor, noRB* NoPai);

/// @brief Realiza uma rotação a direita em um nó;
/// @param noDesbalanceado Nó a sofrer a rotação;
void rotacaoDireita(noRB* noDesbalanceado);

/// @brief Realiza uma rotação a esquerda em um nó;
/// @param noDesbalanceado Nó a sofrer a rotação;
void rotacaoEsquerda(noRB* noDesbalanceado);

/// @brief Define a raiz da árvore RB após a conversão;
/// @param arv Árvore RB a receber a raiz;
/// @param noRaiz Nó a se tornar raiz;
void setRaiz(arvoreRB* arv, noRB* noRaiz);

////////////////////////////////////////////Métodos Auxiliares////////////////////////////////////////////

/// @brief Obtém a raiz de uma árvore RB;
/// @param arv Árvore RB que se busca a raiz;
noRB* retornaRaizRB(arvoreRB* arv);

/// @brief Imprime os elementos de uma árvore RB;
/// @param arv Árvore RB a ser impressa;
void imprimeArvoreRB(arvoreRB *arv);

/// @brief Imprime recursivamente a sub árvore a partir de um nó;
/// @param no Nó raiz da sub-árvore;
/// @param prefixo String a ser printada na próxima iteração;
/// @param ultimoNo Armazena 1 se for o último nó da sub árvore e 0 caso contrário; 
void imprimeNoRB(noRB* no, const char* prefixo, int ultimoNo);

#endif
