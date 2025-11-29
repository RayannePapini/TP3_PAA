#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void alteraChave(char chave[26]) {
    char in[3];
    printf("\nInforme PT->CT (ex: AC): ");

    if (scanf("%2s", in) != 1 || strlen(in) != 2) {
        return;
    }

    char pt = toupper(in[0]);  // plain text
    char ct = toupper(in[1]);  // cipher text

    int idx = ct - 'A';  // índice da letra cifrada

    // impede index inválido
    if (idx < 0 || idx >= ALFABETO) return;

    // impede alterar para algo diferente do que já existe
    if (chave[idx] != DESCONHECIDO && chave[idx] != pt) {
        printf("Erro: '%c' já está mapeado para '%c'\n", ct, chave[idx]);
        return;
    }

    chave[idx] = pt;
}


char *criptografia(char *txt) {
    char alph[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char cripto[26];

    const int X = 3;

    for(int i=0; i<26; i++){
        int calculoCifDesc = (i+X)%26;
        cripto[i] = alph[calculoCifDesc];
    }

    int tamTxt = strlen(txt);
    char *resultCripto = (char*) malloc(tamTxt + 1);
    
    if (!resultCripto) {
        return NULL;
    }
    
    int posCripto = 0;

    for(int i=0; i<tamTxt; i++){
        int encontrado = 0;
        char c = txt[i];

        for(int j=0; j<26; j++){
            if(c == alph[j]){
                resultCripto[posCripto] = cripto[j];
                posCripto++;
                encontrado = 1;
                break;
            }
        }

        if(encontrado == 0){
            resultCripto[posCripto] = c;
            posCripto++;
        }
    }

    resultCripto[posCripto] = '\0';
    return resultCripto;
}

char *geraArqCripto() {
    char nome[100];
    char caminho[200];
    
    while (1) {
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

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
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
        
        buffer[p] = '\0';
        fclose(fp);

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
                realizarCasamentoAproximado(texto, chave);
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

