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

    printf("\nImprimindo árvore 2-3-4:\n");
    imprimeArvore234(arv234);

    arvoreRB* arvRb = alocaArvoreRB();

    /*noRB* no1 = alocaNoRB(1, "V");
    noRB* no2 = alocaNoRB(2, "V");
    noRB* no3 = alocaNoRB(3, "V");
    noRB* no4 = alocaNoRB(4, "V");
    noRB* no5 = alocaNoRB(5, "V");

    insereNoRB(arvRb, no1);
    insereNoRB(arvRb, no2);
    insereNoRB(arvRb, no3);
    insereNoRB(arvRb, no4);
    insereNoRB(arvRb, no5);*/

    printf("\nImprimindo árvore Rubro-Negra\n");

    noRB* raizArvoreRB = converte234(obtemRaiz234(arv234));
    setRaiz(arvRb, raizArvoreRB);

    imprimeArvoreRB(arvRb);
}