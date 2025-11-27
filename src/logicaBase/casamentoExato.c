#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void realizarCasamentoExato(const char *textoCriptografado, char chave[ALFABETO]) {
    char padraoCT[100];
    char padraoPT[100];

    printf("\n=== Casamento Exato de Caracteres ===\n");
    printf("Esta opcao procura um padrao no texto criptografado e aplica a traducao fornecida.\n");
    
    printf("Digite o trecho do texto criptografado (Ex: QWERTY): ");
    if (scanf("%99s", padraoCT) != 1) return;
    for (int i = 0; padraoCT[i]; i++) padraoCT[i] = toupper(padraoCT[i]);
    if (strstr(textoCriptografado, padraoCT) == NULL) {
        printf("ERRO: O padrao '%s' nao foi encontrado no texto criptografado.\n", padraoCT);
        return;
    }
    printf("Digite a traducao EXATA para '%s' (Ex: SYSTEM): ", padraoCT);
    if (scanf("%99s", padraoPT) != 1) return;
    for (int i = 0; padraoPT[i]; i++) padraoPT[i] = toupper(padraoPT[i]);
    if (strlen(padraoCT) != strlen(padraoPT)) {
        printf("ERRO: O tamanho do texto criptografado (%lu) nao corresponde ao tamanho da traducao (%lu).\n", 
               (unsigned long)strlen(padraoCT), (unsigned long)strlen(padraoPT));
        return;
    }
    int atualizacoes = 0;
    int conflitos = 0;

    printf("\nProcessando mapeamento...\n");
    for (int i = 0; i < (int)strlen(padraoCT); i++) {
        char ct = padraoCT[i];
        char pt = padraoPT[i];

        if (!isalpha(ct) || !isalpha(pt)) continue;

        int indice = ct - 'A';
        if (chave[indice] != DESCONHECIDO && chave[indice] != pt) {
            printf("CONFLITO: A letra '%c' ja esta mapeada para '%c'. Ignorando nova tentativa '%c'.\n", 
                   ct, chave[indice], pt);
            conflitos++;
        } else if (chave[indice] == DESCONHECIDO) {
            int letraJaUsada = 0;
            for(int k=0; k<ALFABETO; k++){
                if(chave[k] == pt && k != indice){
                    letraJaUsada = 1;
                    break;
                }
            }

            if(letraJaUsada){
                printf("AVISO: A letra PT '%c' ja esta sendo usada por outra letra CT. Mapeamento mantido, mas verifique.\n", pt);
            }

            chave[indice] = pt;
            atualizacoes++;
        }
    }

    if (atualizacoes > 0) {
        printf("\nSUCESSO: %d letras foram mapeadas/atualizadas na chave.\n", atualizacoes);
        if (conflitos > 0) printf("ATENCAO: Houve %d conflitos ignorados.\n", conflitos);

        exibeEstado(textoCriptografado, chave);
    } else if (conflitos == 0) {
        printf("\nNenhuma alteracao necessaria (as letras ja estavam mapeadas corretamente).\n");
    } else {
        printf("\nNenhuma alteracao feita devido a conflitos.\n");
    }
}