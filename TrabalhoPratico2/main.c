#include "Arvore234/arvore234.h"
#include "ArvoreRubroNegra/arvoreRB.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    arvore234* arv234 = alocaArvore234();
    arvoreRB* arvRB = alocaArvoreRB();

    int converteu = 0;
    int acao, chave;
    char nomeArquivo[50];

    printf("Digite o nome do arquivo: ");
    scanf("%s", &nomeArquivo);

    if(!preencheArvore(arv234, nomeArquivo)){
        printf("Erro na leitura do arquivo\n");
        return 0;
    }
    
    while(!converteu){
        printf("------------------------------------------\n");

        printf("\nEscolha uma ação:\n\n");
        printf("1. Inserir elemento na árvore 2-3-4\n");
        printf("2. Remover elemento da árvore 2-3-4\n");
        printf("3. Imprimir árvore 2-3-4\n");
        printf("4. Converter em uma árvore rubro-negra\n");
        printf("5. Sair\n>");

        scanf("%d", &acao);

        printf("\n------------------------------------------\n");

        switch(acao){
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                insereChaveArvore(arv234, chave);
                printf("Elemento inserido com sucesso\n");
                break;

            case 2:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                removeChaveArvore(arv234, chave);
                break;

            case 3:
                printf("Imprimindo árvore 2-3-4...\n");
                imprimeArvore234(arv234);
                break;

            case 4:
                printf("Convertendo em uma árvore rubro-negra...\n");
                setRaiz(arvRB, converte234(obtemRaiz234(arv234), NULL));
                converteu = 1;
                printf("Conversão concluida!\n");
                break;

            case 5:
                printf("Encerrando programa...\n");
                return 0;

            default:
                printf("Ação inválida.\n");
        }
    }

    while(1){
        printf("------------------------------------------\n");

        printf("\nEscolha uma ação:\n\n");
        printf("1. Inserir elemento na árvore rubro-negra\n");
        printf("2. Remover elemento da árvore rubro-negra\n");
        printf("3. Imprimir árvore rubro-negra\n");
        printf("4. Sair\n>");

        scanf("%d", &acao);

        printf("\n------------------------------------------\n");

        switch(acao){
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                noRB* novoNo = alocaNoRB(chave, 'V');
                insereNoRB(arvRB, novoNo);
                printf("Elemento inserido com sucesso\n");
                break;

            case 2:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                removeNoRB(arvRB, chave);
                break;

            case 3:
                printf("Imprimindo árvore rubro-negra...\n");
                imprimeArvoreRB(arvRB);
                break;

            case 4:
                printf("Encerrando programa...\n");
                return 0;

            default:
                printf("Ação inválida.\n");
        }
    }
}