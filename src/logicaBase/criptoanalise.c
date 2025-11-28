#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char ORDEM_PT[] = "AEOSRINDMUTCLPVQGHBFZJXKYW"; 

int comparaEntradasFrequencia(const void *a, const void *b) {
    EntradaFrequencia *entradaA = (EntradaFrequencia *)a;
    EntradaFrequencia *entradaB = (EntradaFrequencia *)b;
    
    if (entradaB->contagem != entradaA->contagem) {
        return entradaB->contagem - entradaA->contagem;
    }
    return entradaA->letra - entradaB->letra;
}

void inicializaChave(char chave[ALFABETO]) {
    for (int i = 0; i < ALFABETO; i++) {
        chave[i] = DESCONHECIDO; 
    }
}

void decifraTexto(const char *texto_criptografado, const char *chave, char *texto_decifrado) {
    int tamanho = strlen(texto_criptografado);
    for (int i = 0; i < tamanho; i++) {
        char c = texto_criptografado[i];
        if (isupper(c)) {
            int indice = c - 'A';
            if (chave[indice] != DESCONHECIDO) {
                texto_decifrado[i] = chave[indice];
            } else {
                texto_decifrado[i] = c;
            }
        } else {
            texto_decifrado[i] = c; 
        }
    }
    texto_decifrado[tamanho] = '\0';
}

void exibeEstado(const char *texto_criptografado, const char *chave) {
    int tamanho = strlen(texto_criptografado);
    char *texto_decifrado = (char*) malloc(tamanho + 1);
    if (!texto_decifrado) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    decifraTexto(texto_criptografado, chave, texto_decifrado);

    printf("\nTexto criptografado\n");
    printf("%s\n", texto_criptografado);

    printf("\nChave\n");
    printf("CT: ");
    for (int i = 0; i < ALFABETO; i++) {
        printf("%c", 'A' + i);
    }
    printf("\n");
    printf("PT: ");
    for (int i = 0; i < ALFABETO; i++) {
        printf("%c", chave[i]);
    }
    printf("\n");

    printf("\nTexto parcialmente decifrado\n");
    printf("%s\n", texto_decifrado);
    
    printf("    ");
    for (int i = 0; i < tamanho; i++) {
        char c = texto_criptografado[i];
        if (isupper(c) && chave[c - 'A'] != DESCONHECIDO) {
            printf("_");
        } else if (isspace(c)) {
             printf(" ");
        } else {
            printf(" ");
        }
    }
    printf("\n");
    
    free(texto_decifrado);
}

void analiseFrequencia(const char *texto_criptografado, char chave[ALFABETO]) {
    int contagens[ALFABETO] = {0};
    int total_letras = 0;
    
    for (int i = 0; texto_criptografado[i] != '\0'; i++) {
        char c = texto_criptografado[i];
        if (isupper(c)) {
            contagens[c - 'A']++;
            total_letras++;
        }
    }

    if (total_letras == 0) {
        printf("\nO texto criptografado nao contem letras para analise.\n");
        return;
    }

    EntradaFrequencia entradas[ALFABETO];
    for (int i = 0; i < ALFABETO; i++) {
        entradas[i].letra = 'A' + i;
        entradas[i].contagem = contagens[i];
        entradas[i].frequencia = (double)contagens[i] / total_letras * 100.0;
    }

    qsort(entradas, ALFABETO, sizeof(EntradaFrequencia), comparaEntradasFrequencia);

    printf("\nAnalise de Frequencia do Texto Criptografado\n");
    printf("Total de letras: %d\n", total_letras);
    printf("----------------------------------------\n");
    printf("| Letra | Cont. | Freq.  |\n");
    printf("|-------|-------|--------|\n");
    for (int i = 0; i < ALFABETO; i++) {
        if (entradas[i].contagem > 0) {
            printf("|   %c   |  %4d | %5.2f%% |\n", entradas[i].letra, entradas[i].contagem, entradas[i].frequencia);
        }
    }
    printf("----------------------------------------\n");

    printf("\nTentativa de 'Chute' Baseada em Frequencia\n");
    printf("A heuristica mais simples e: as letras CT mais frequentes mapeiam para as letras PT mais frequentes.\n");
    printf("Ordem PT: %s\n", ORDEM_PT);

    int mapeamentos_feitos = 0;
    
    printf("\nMapeamento Sugerido:\n");
    for (int i = 0; i < ALFABETO && i < (int)strlen(ORDEM_PT); i++) {
        char char_ct = entradas[i].letra;
        if (entradas[i].contagem > 0 && chave[char_ct - 'A'] == DESCONHECIDO) {
            
            char char_pt = ORDEM_PT[i];
            
            chave[char_ct - 'A'] = char_pt;
            
            printf("  %c (CT: %.2f%%) -> %c (PT)\n", char_ct, entradas[i].frequencia, char_pt);
            mapeamentos_feitos++;
        }
    }
    
    if (mapeamentos_feitos > 0) {
        printf("\nChave de Decifracao ATUALIZADA apos Chute ---\n");
        exibeEstado(texto_criptografado, chave);
    } else {
        printf("\nNenhum novo mapeamento foi sugerido ou todas as letras ja foram mapeadas.\n");
    }
}

void export(char *textoCriptografado, char *chave){
    int tam = strlen(textoCriptografado);
    char *textoFinal = (char*) malloc(tam + 1);
    decifraTexto(textoCriptografado, chave, textoFinal);

    char nomeArq[100];
    char caminhoArq[150];
    FILE *file = NULL;

    while(1){
        printf("\nDigite o nome do arquivo(sem extensoes) onde o resultado sera salvo!\n");
        printf("O arquivo poedrá ser encontrado na no seguinte diretório: >TP3_PAA/testes/resultTestes/nomeArquivo.txt\n");
        scanf("%s", nomeArq);
        sprintf(caminhoArq, "testes/resultTestes/%s.txt", nomeArq);

        // abre arquivo no modo leitura para identificar existência no diretório
        file = fopen(caminhoArq, "r");

        // se arquivo não é nulo isso significa que já existe um arquivo escrito no diretório passado
        // portanto, devemos escolher outro nome pro arquivo
        if(file != NULL){
            fclose(file);
            printf("Ja existe um arquivo com o nome %s, tente outro nome!\n", nomeArq);
            continue;
        }

        // se não existe arquivo podemos abrir no modo leitura
        file = fopen(caminhoArq, "w");
        if(file != NULL){
            // caso entre nesta condição o arquivo foi criado com sucesso

            // precisamos agora escrever o resultado em file
            fprintf(file, textoFinal);
            printf("Seu arquivo foi criado e registrado com sucesso!\n");
            fclose(file);

            break;
        } else{
            printf("Houve um problema inexperado! Tente novamente\n");
            continue;
        }
    }

    printf("Resultado esperado no arquivo:\n");
    printf("%s\n", textoCriptografado);
}
