#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void realizarCasamentoAproximado(const char *textoCriptografado, const char *chave) {
    char padrao[200];
    int tolerancia;
    int i, j;

    printf("\nCasamento aproximado de caracteres\n");
    printf("Digite o padrao (palavra que voce quer procurar): ");
    scanf("%199s", padrao);

    // Converte padrao para maiúsculas
    for (i = 0; padrao[i] != '\0'; i++) {
        padrao[i] = toupper(padrao[i]);
    }

    printf("Digite a tolerancia (quantidade maxima de substituicoes permitidas): ");
    scanf("%d", &tolerancia);

    if (tolerancia < 0) {
        tolerancia = 0;
    }

    // Cria texto parcialmente decifrado 
    int n = strlen(textoCriptografado);
    char *decifrado = (char *) malloc(n + 1);

    if (decifrado == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    for (i = 0; i < n; i++) {
        char c = textoCriptografado[i];

        if (c >= 'A' && c <= 'Z') {
            if (chave[c - 'A'] != DESCONHECIDO) {
                decifrado[i] = chave[c - 'A'];
            } else {
                decifrado[i] = c;
            }
        } else {
            decifrado[i] = c;
        }
    }
    decifrado[n] = '\0';

    int m = strlen(padrao);
    int ocorrencias = 0;

    if (m > 63) {
        printf("Erro: Padrao muito grande.\n");
        free(decifrado);
        return;
    }

    unsigned long mascara[256];

    for (i = 0; i < 256; i++) {
        mascara[i] = 0;
    }

    for (i = 0; i < m; i++) {
        // Coloca o bit 1 na posição i para a letra correspondente
        mascara[(unsigned char)padrao[i]] |= (1UL << i);
    }

    // Inicialização dos estados (R)
    unsigned long R[tolerancia + 1];
    unsigned long R_novo[tolerancia + 1];

    for (j = 0; j <= tolerancia; j++) {
        R[j] = 0;
    }

    unsigned long bit_sucesso = (1UL << (m - 1));

    //agora vai fazer a busca no texto
    for (i = 0; i < n; i++) {
        R_novo[0] = ((R[0] << 1) | 1) & mascara[(unsigned char)decifrado[i]];

        // Calcula os estados com erros 
        for (j = 1; j <= tolerancia; j++) {
            unsigned long casou = ((R[j] << 1) | 1) & mascara[(unsigned char)decifrado[i]];
            unsigned long substituiu = ((R[j-1] << 1) | 1);

            R_novo[j] = casou | substituiu;
        }

        for (j = 0; j <= tolerancia; j++) {
            R[j] = R_novo[j];
        }

        // Verifica se o bit final está ligado no nível máximo de tolerância
        if (R[tolerancia] & bit_sucesso) {
            ocorrencias++;
            int posicao_inicio = i - m + 1;
            
            if (posicao_inicio >= 0) {  
                printf("Ocorrencia %d encontrada na posicao %d: ", ocorrencias, posicao_inicio);
                
                for (j = 0; j < m; j++) {
                    putchar(decifrado[posicao_inicio + j]);
                }
                printf("\n");
            }
        }
    }

    if (ocorrencias == 0) {
        printf("Nenhuma ocorrencia encontrada.\n");
    } else {
        printf("\nTotal de ocorrencias: %d\n", ocorrencias);
    }

    free(decifrado);

    printf("\nUse a opcao 5 do menu para atualizar a chave caso voce identifique novas letras.\n");
}