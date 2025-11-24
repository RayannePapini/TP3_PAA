#include <stdio.h>
#include "estruturas/estruturas.h"

int main(){
    int saida;
    while(1){
        printf("Voce ira analisar textos criptografados agora!\n");
        printf("Caso nao queira continuar no sistema digite 1 para sair!\n");
        printf("Qualquer outro valor indicará que voce deseja continuar no programa!\n");
        scanf("%d", &saida);
        if(saida == 1){
            break;
        }

        char *textoCriptografado = geraArqCripto();
        printf("%s\n", textoCriptografado);

        menu(textoCriptografado); 
    }
    

    return 0;
}