#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int shiftAndExato(const char *texto, const char *padrao) {
    int m = strlen(padrao);
    int n = strlen(texto);
    if (m > 63) {
        printf("Erro: Muito longo para Shift-And simples\n");
        return 0;
    }
    unsigned long R = 0;
    unsigned long mascara[256];

    for (int i = 0; i < 256; i++) {
        mascara[i] = 0;
    }
    for (int i = 0; i < m; i++) {
        mascara[(unsigned char)padrao[i]] |= (1UL << i);
    }
    int ocorrencias = 0;
    printf("Procurando padrao '%s'...\n", padrao);
    for (int i = 0; i < n; i++) {
        R = ((R << 1) | 1) & mascara[(unsigned char)texto[i]];
        if (R & (1UL << (m - 1))) {
            ocorrencias++;
            printf(" > Ocorrencia %d encontrada na posicao %d\n", ocorrencias, i - m + 1);
        }
    }
    return ocorrencias;
}

void realizarCasamentoExato(const char *textoCriptografado, char chave[ALFABETO]) {
    char padraoCT[100];
    printf("\n___Casamento exato de caracteres (Shift-And)___\n");
    printf("Busca por padroes no TEXTO CRIPTOGRAFADO\n");
    printf("Digite o trecho CRIPTOGRAFADO (Ex: QWERTY): ");
    if (scanf("%99s", padraoCT) != 1) return;
    for (int i = 0; padraoCT[i]; i++) padraoCT[i] = toupper(padraoCT[i]);
    int qtd = shiftAndExato(textoCriptografado, padraoCT);
    if (qtd == 0) {
        printf("O padrao '%s' NAO foi encontrado.\n", padraoCT);
        return;
    }
    printf("\nOcorrencias: %d\n", qtd);
    printf("Use a Opção 5 (Alterar chave de criptografia) para aplicar suas suspeitas de mapeamento.\n");
}