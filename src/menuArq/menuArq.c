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
    if (scanf("%2s", in) != 1 || strlen(in) != 2) return;
    char o = toupper(in[0]);
    char m = toupper(in[1]);
    int idx = m - 'A';
    if (idx < 0 || idx >= ALFABETO) return;
    if (chave[idx] != DESCONHECIDO && chave[idx] != o) return;
    chave[idx] = o;
}

char *criptografia(char *txt) {
    if (SHIFT_GLOBAL == -1) {
        srand((unsigned)time(NULL));
        SHIFT_GLOBAL = (rand() % 25) + 1;
        criptografaCorpus12(SHIFT_GLOBAL);
    }

    int n = (int)strlen(txt);
    char *out = malloc(n + 1);
    if (!out) return NULL;
    int pos = 0;
    for (int i = 0; i < n; i++) {
        char c = txt[i];
        if (c >= 'A' && c <= 'Z')
            out[pos++] = (char)(((c - 'A' + SHIFT_GLOBAL) % 26) + 'A');
        else if (c >= 'a' && c <= 'z')
            out[pos++] = (char)(((c - 'a' + SHIFT_GLOBAL) % 26) + 'A');
        else
            out[pos++] = c;
    }
    out[pos] = '\0';
    return out;
}

char *geraArqCripto() {
    char nome[100];
    char caminho[200];
    while (1) {
        printf("\nArquivo de entrada (sem extensao): ");
        if (scanf("%99s", nome) != 1) {
            while (getchar() != '\n');
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
        if (!buffer) { fclose(fp); return NULL; }
        int p = 0, ch;
        while ((ch = fgetc(fp)) != EOF) buffer[p++] = (char)toupper(ch);
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
        if (scanf("%d", &op) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');
        if (op == 1) exibeEstado(texto, chave);
        else if (op == 2) {
            int x;
            printf("\n1. Apenas arquivo\n");
            printf("2. Usar 12 arquivos\n");
            if (scanf("%d", &x) != 1) { while (getchar() != '\n'); continue; }
            while (getchar() != '\n');
            if (x == 1) analiseFrequencia(texto);
            else if (x == 2) analiseCorpus12();
        }
        else if (op == 3) realizarCasamentoExato(texto);
        else if (op == 4) printf("Indisponivel.\n");
        else if (op == 5) alteraChave(chave);
        else if (op == 6) {
            export(chave);
            free(texto);
            return;
        }
    }
}
