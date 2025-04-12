#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TreeSort.h"

int main(){
    FILE* arq;
    arq = fopen("resultados/resultadoTreeSort.csv" , "w");
    fprintf(arq , "Index,Tipo de Ordenação,Quantidade de posições de vetor,Tempo de execução(seg)\n");

    int quantidade_entrada[] = {2000 , 10000 , 50000, 250000 , 500000 , 1000000};

    for(int entrada = 0 ; entrada < (sizeof(quantidade_entrada)/sizeof(int)) ; entrada++){
        clock_t inicio , fim;

        inicio = clock();

        printf("Ordenação de número %d : Quantidade posições do vetor será ordenado:%d \n" , entrada + 1 , quantidade_entrada[entrada]);

        int* vetor = preencherVetor(quantidade_entrada[entrada]);
        lista* lista = crialista();

        if(vetor == NULL){
            printf("ERROR");
            return -1;
        }

        if(lista == NULL){
            printf("ERROR");
            return -1;
        }

        for(int i = 0 ; i < quantidade_entrada[entrada] ; i++){
            no* novo = criano(vetor[i]);

            if(novo == NULL){
                printf("ERROR");
                i = quantidade_entrada[entrada];
                return -1;
            }

            inserirLista(lista , novo);
        }

        int* vetorOrdem = (int*)malloc(sizeof(int) * quantidade_entrada[entrada]);
        int index = 0;

        emOrdem(lista->pai , vetorOrdem , &index);

        fim = clock();
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("Tempo de execução é %.7f segundos\n\n" , tempo);
        fprintf(arq , "%d , Tree Sort , %d , %.7f \n" ,entrada + 1, quantidade_entrada[entrada] , tempo);
    }
    
    fclose(arq);
    return 0;
}