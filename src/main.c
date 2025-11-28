#include <stdio.h>
#include "estruturas/estruturas.h"

int main() {
    int saida;

    while (1) {
        printf("Voce ira analisar textos criptografados agora!\n");
        printf("Digite 1 para sair, ou qualquer outro valor para continuar:\n");
        scanf("%d", &saida);

        if (saida == 1) break;

        char *textoCriptografado = geraArqCripto();
        printf("\nTexto criptografado:\n%s\n", textoCriptografado);

        menu(textoCriptografado); 
    }

    return 0;
}
