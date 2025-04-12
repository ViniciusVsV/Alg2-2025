#include "Geradoras.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void imprimeVetor(int *vet, int tam) {
    for(int i = 0; i < tam; i++) {
        printf("%d ", vet[i]);
    }

    printf("\n");
}

int *geraVetor(int tam, int porcentagemOrdenada) {
    //Cálculo do tamanho da parte ordenada do vetor;
    int tamOrdenado = tam * porcentagemOrdenada / 100;

    //Alocação dinâmica do vetor;
    int *vet = (int *) malloc(sizeof(int) * tam);
    if(!vet) return NULL;

    //Preenche o vetor em ordem DECRSCENTE;
    if(porcentagemOrdenada == -1) {
        for(int i = 0; i < tam; i++) {
            vet[i] = tam - i;
        }

        imprimeVetor(vet, tam);

        return vet;
    }

    //Preenchimento da parte ordenada do vetor;
    for(int i = 0; i < tamOrdenado; i++) {
        vet[i] = i;
    }

    //Preenchimento da parte aleatória do vetor;
    srand(time(NULL));

    for(int i = tamOrdenado; i < tam; i++) {
        if(tamOrdenado > 0) vet[i] = vet[tamOrdenado - 1] + rand(); //Caso o vetor tenha uma parte ordenada;
        else vet[i] = rand(); //Caso o vetor seja TOTALMENTE desordenado;
    }

    imprimeVetor(vet, tam);

    return vet;
}











