#include <btree.h>
#include <stdio.h>

int main(){
    btree* arv = alocaBtree();
    
    while(1) {
        int acao, chave;

        printf("Escolha uma ação:\n");
        printf("1. Inserir chave\n");
        printf("2. Remover chave\n> ");
        scanf("%d", &acao);

        switch(acao) {
            case 1:
                printf("Digite a chave a ser inserida: ");
                scanf("%d", &chave);
                insereBtree(arv, chave);
                printf("\nÁrvore após inserção:\n");
                imprimeBtree(arv);
                break;
            case 2:
                printf("Digite a chave a ser removida: ");
                scanf("%d", &chave);
                removeBtree(arv, chave);
                printf("\nÁrvore após remoção:\n");
                imprimeBtree(arv);
                break;
            default:
                printf("Ação inválida.\n");
        }
    }
}