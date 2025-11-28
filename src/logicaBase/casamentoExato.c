#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// funcao do algoritmo Shift-And para busca exata de padroes
int shiftAndExato(const char *texto, const char *padrao) {
    int m = strlen(padrao); // comprimento do padrao
    int n = strlen(texto); //comprimento do texto
    if (m > 63) { // o algoritmo Shift-And simples e limitado pelo tamanho do registrador (unsigned long, 64 bits)
        printf("Erro: Muito longo para Shift-And simples\n");
        return 0;
    }
    unsigned long R = 0; // registrador de estado, um bit setado indica um prefixo do padrao que termina na posicao atual
    unsigned long mascara[256]; // tabela de mascaras de bits para todos os 256 caracteres possiveis

    for (int i = 0; i < 256; i++) { // inicializa a tabela com zeros
        mascara[i] = 0;
    }
    for (int i = 0; i < m; i++) { // pre-processamento, constroi a tabela de mascaras
        mascara[(unsigned char)padrao[i]] |= (1UL << i);
    }
    int ocorrencias = 0;
    printf("Procurando padrao '%s'...\n", padrao);
    
    for (int i = 0; i < n; i++) { // fase de Busca, percorre o texto, atualizando o registrador de estado R
        R = ((R << 1) | 1) & mascara[(unsigned char)texto[i]];
        if (R & (1UL << (m - 1))) {
            ocorrencias++;
            printf(" > Ocorrencia %d encontrada na posicao %d\n", ocorrencias, i - m + 1);
        }
    }
    return ocorrencias;
}

void realizarCasamentoExato(const char *textoCriptografado) { // Funcao de interface para solicitar o padrao e executar a busca
    char padraoCT[100];
    printf("\nCasamento exato de caracteres (Shift-And)\n");
    printf("Busca por padroes no TEXTO CRIPTOGRAFADO\n");
    printf("Digite o trecho CRIPTOGRAFADO (Ex: QWERTY): ");
    
    if (scanf("%99s", padraoCT) != 1){
        return;
    }
    for (int i = 0; padraoCT[i]; i++){
        padraoCT[i] = toupper(padraoCT[i]); // converte o padrao para MAIUSCULAS para garantir busca
    }
    
    int qtd = shiftAndExato(textoCriptografado, padraoCT); // executa o algoritmo Shift-And
    
    if (qtd == 0) {
        printf("O padrao '%s' NAO foi encontrado.\n", padraoCT);
        return;
    }
    printf("\nOcorrencias: %d\n", qtd);
    
    printf("Use a Opcao 5 (Alterar chave de criptografia) para aplicar suas suspeitas de mapeamento.\n"); // sugestao de proxima ação para o usuario
}
