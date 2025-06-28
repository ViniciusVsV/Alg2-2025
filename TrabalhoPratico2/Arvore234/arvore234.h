#ifndef ARVORE234
#define ARVORE234

typedef struct arvore234 arvore234;
typedef struct no234 no234;

/// @brief Aloca um novo objeto do tipo arvore234
/// @return Nova árvore criada
arvore234* alocaArvore234();

/// @brief Aloca um novo objeto do tipo no234
/// @param folha Booleana que dita se o nó criado é folha ou não
/// @return Novo nó criado
no234* alocaNo234(int folha);

/// @brief Insere um novo elemento em uma árvore 2-3-4
/// @param arv Árvore na qual o elemento será inserido
/// @param chave Elemento a ser inserido na árvore
void insereChaveArvore(arvore234* arv, int chave);

/// @brief Insere um elemento em um nó folha específico de uma árvore 2-3-4
/// @param noAlvo Nó folha no qual o elemento será inserido
/// @param chave Elemento a ser inserido no nó folha
void insereChaveNoFolha(no234* noAlvo, int chave);

/// @brief Insere um elemento em um nó interno específico de uma árvore 2-3-4
/// @param noAlvo Nó interno no qual o elemento será inserido
/// @param chave Elemento a ser inserido no nó folha
/// @param filhoEsquerdo Filho esquerdo do novo elemento nó a ser inserido
/// @param filhoDireito Filho direito do novo elemento a ser inserido
void insereChaveNoInterno(no234* noAlvo, int chave, no234* filhoEsquerdo, no234* filhoDireito);

/// @brief Divide um nó cheio em dois para inserção de um novo elemento
/// @param arv Árvore na qual a operação é realizada
/// @param pai Pai do nó a ser dividido
/// @param noCheio Nó cheio que deverá ser dividido
/// @param chave Valor do elemento a ser inserido no nó cheio
/// @return Nó correto para inserção do elemento após a divisão
no234* divideNo(arvore234* arv, no234* pai, no234* noCheio, int chave);

/// @brief Obtém a raiz de uma dada árvore 2-3-4
/// @param arv Árvore em questão
/// @return Nó raiz da árvore
no234* obtemRaiz234(arvore234* arv);

/// @brief Obtém a quantidade de splits realizado em uma árvore 2-3-4
/// @param arv Árvore em questão
/// @return Quantidade de divisões feitas
int obtemQtdSplit(arvore234* arv);

/// @brief Obtém a altura de uma dada árvore 2-3-4
/// @param arv Árvore em questão
/// @return Altura da árvore
int obtemAltura234(arvore234* arv);

/// @brief Imprime os elementos de uma árvore 2-3-4
/// @param arv Árvore em questão
void imprimeArvore234(arvore234 *arv);

/// @brief Imprime as chaves de um nó de uma árvore 2-3-4
/// @param no Nó em questão
/// @param prefixo 
/// @param is_last 
void imprimeNo234(no234* no, const char *prefix, int is_last);

#endif