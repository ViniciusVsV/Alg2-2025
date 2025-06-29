#include "Arvore234/arvore234.h"
#include "ArvoreRubroNegra/arvoreRB.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    arvore234* arv234 = alocaArvore234();

    insereChaveArvore(arv234, 1);
    insereChaveArvore(arv234, 2);
    insereChaveArvore(arv234, 3);
    insereChaveArvore(arv234, 4);
    insereChaveArvore(arv234, 5);
    insereChaveArvore(arv234, 6);
    insereChaveArvore(arv234, 7);
    insereChaveArvore(arv234, 8);
    insereChaveArvore(arv234, 9);
    insereChaveArvore(arv234, 10);

    printf("\nImprimindo árvore 2-3-4...\n");
    imprimeArvore234(arv234);

    printf("\nRemovendo elementos da árvore 2-3-4...\n");
    removeChaveArvore(arv234, 1);
    removeChaveArvore(arv234, 2);
    removeChaveArvore(arv234, 3);
    removeChaveArvore(arv234, 4);
    removeChaveArvore(arv234, 6);
    removeChaveArvore(arv234, 5);
    removeChaveArvore(arv234, 8);
    removeChaveArvore(arv234, 9);
    removeChaveArvore(arv234, 7);
    removeChaveArvore(arv234, 10);

    printf("\nImprimindo árvore 2-3-4 após as remoções...\n");
    imprimeArvore234(arv234);

    /*printf("\nImprimindo árvore 2-3-4 após a remoção...\n");
    imprimeArvore234(arv234);

    arvoreRB* arvRb = alocaArvoreRB();

    printf("\nImprimindo árvore Rubro-Negra...\n");

    noRB* raizArvoreRB = converte234(obtemRaiz234(arv234));
    setRaiz(arvRb, raizArvoreRB);

    imprimeArvoreRB(arvRb);*/
}