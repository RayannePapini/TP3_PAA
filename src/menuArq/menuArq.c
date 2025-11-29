#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int SHIFT_GLOBAL = -1;

void alteraChave(char chave[26]) {
    char in[3];
    printf("\nInforme PT->CT (ex: AC): ");
    
    if (scanf("%2s", in) != 1 || strlen(in) != 2) {
        return;
    }
    
    char o = toupper(in[0]);
    char m = toupper(in[1]);
    int idx = m - 'A';
    
    if (idx < 0 || idx >= ALFABETO) {
        return;
    }
    
    if (chave[idx] != DESCONHECIDO && chave[idx] != o) {
        return;
    }
    
    chave[idx] = o;
}

char *criptografia(char *txt) {
    if (SHIFT_GLOBAL == -1) {
        SHIFT_GLOBAL = 3;
        criptografaCorpus12(SHIFT_GLOBAL);
    }

    int n = (int)strlen(txt);
    char *out = malloc(n + 1);
    
    if (!out) {
        return NULL;
    }
    
    int pos = 0;
    
    for (int i = 0; i < n; i++) {
        char c = txt[i];
        
        if (c >= 'A' && c <= 'Z') {
            out[pos++] = (char)(((c - 'A' + SHIFT_GLOBAL) % 26) + 'A');
        } 
        else if (c >= 'a' && c <= 'z') {
            out[pos++] = (char)(((c - 'a' + SHIFT_GLOBAL) % 26) + 'A');
        } 
        else {
            out[pos++] = c;
        }
    }
    
    out[pos] = '\0';
    return out;
}

char *geraArqCripto() {
    char nome[100];
    char caminho[200];
    
    while (1) {
        // carregando arquivo de acordo com entrada do usuário
        printf("\nDigite o nome do arquivo de entrada para criptografia(sem extensao):\n");
        
        if (scanf("%99s", nome) != 1) {
            while (getchar() != '\n'){

            }
            continue;
        }
        
        snprintf(caminho, sizeof(caminho), "testes/%s.txt", nome);
        FILE *fp = fopen(caminho, "r");
        
        if (!fp) {
            snprintf(caminho, sizeof(caminho), "%s.txt", nome);
            fp = fopen(caminho, "r");
        }
        
        if (!fp) {
            printf("Arquivo invalido.\n");
            continue;
        }
        
        // encontrando tamanho do arquivo
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        //volta file pro início para a leitura
        fseek(fp, 0, SEEK_SET);
        
        char *buffer = malloc(size + 1);        
        if (!buffer) { 
            fclose(fp); 
            return NULL; 
        }
        
        int p = 0, ch;
        
        while ((ch = fgetc(fp)) != EOF) {
            buffer[p++] = (char)toupper(ch);
        }
        
        // garante que string está fechada
        buffer[p] = '\0';
        fclose(fp);

        // criptografar
        char *cript = criptografia(buffer);
        free(buffer);
        
        return cript;
    }
}

void menu(char *texto) {
    char chave[ALFABETO];
    inicializaChave(chave);
    
    while (1) {
        int op;
        printf("\nMENU\n");
        printf("1. Estado atual\n");
        printf("2. Analise de Frequencia\n");
        printf("3. Casamento Exato\n");
        printf("4. Casamento Aproximado\n");
        printf("5. Alterar chave manualmente\n");
        printf("6. Exportar e sair\n");
        printf("Escolha: ");
        
        // lê opção e limpa o buffer
        if (scanf("%d", &op) != 1) { 
            while (getchar() != '\n'); 
            continue; 
        }
        
        while (getchar() != '\n');
        
        switch (op) {
            case 1:
                exibeEstado(texto, chave);
                break;
            case 2: {
                // opções para 1 ou 12 arquivos
                int x;
                printf("\n1. Apenas arquivo\n");
                printf("2. Usar 12 arquivos\n");
                
                if (scanf("%d", &x) != 1) { 
                    while (getchar() != '\n'){
                        
                    }
                    break; 
                }
                
                while (getchar() != '\n');
                
                switch (x) {
                    case 1:
                        analiseFrequencia(texto);
                        break;
                    case 2:
                        analiseCorpus12();
                        break;
                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
                break;
            }
            case 3:
                realizarCasamentoExato(texto);
                break;
            case 4:
                printf("Indisponivel.\n");
                break;
            case 5:
                alteraChave(chave);
                break;
            case 6:
                export(texto, chave);
                free(texto);
                return;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }
}
