#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { char letra; double freq_padrao; } FreqPadrao;

const FreqPadrao TABELA_WIKI[ALFABETO] = {
    {'A',14.63},{'E',12.57},{'O',10.73},{'S',7.81},{'R',6.53},
    {'I',6.18},{'N',5.05},{'D',4.99},{'M',4.74},{'U',4.63},
    {'T',4.34},{'C',3.88},{'L',2.78},{'P',2.52},{'V',1.67},
    {'G',1.30},{'H',1.28},{'Q',1.20},{'B',1.04},{'F',1.02},
    {'Z',0.47},{'J',0.40},{'X',0.21},{'K',0.02},{'W',0.01},
    {'Y',0.01}
};

typedef struct { char letra; int contagem; double frequencia; } EntradaFrequencia;

int comparaEntradasFrequencia(const void *a, const void *b) {
    const EntradaFrequencia *A = a;
    const EntradaFrequencia *B = b;
    return B->contagem - A->contagem;
}

void inicializaChave(char chave[ALFABETO]) {
    for (int i = 0; i < ALFABETO; i++) chave[i] = DESCONHECIDO;
}

void decifraTexto(const char *cript, const char *chave, char *decifrado) {
    int n = (int)strlen(cript);
    for (int i = 0; i < n; i++) {
        char c = cript[i];
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            decifrado[i] = (chave[idx] == DESCONHECIDO) ? c : chave[idx];
        } else decifrado[i] = c;
    }
    decifrado[n] = '\0';
}

void exibeEstado(const char *cript, const char *chave) {
    printf("\nEstado Atual\n");
    printf("Texto Criptografado:\n%s\n", cript);
    printf("\nChave Atual:\n");
    printf("CT: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("PT: ");
    for (int i = 0; i < ALFABETO; i++) printf("%c", chave[i]);
    printf("\n\nTexto Parcialmente Decifrado:\n");
    int n = (int)strlen(cript);
    for (int i = 0; i < n; i++) {
        char c = cript[i];
        if (c >= 'A' && c <= 'Z') {
            if (chave[c - 'A'] != DESCONHECIDO)
                printf("%s%c%s", COR_VERDE, chave[c - 'A'], COR_RESET);
            else
                printf("%s%c%s", COR_VERMELHO, c, COR_RESET);
        } else printf("%c", c);
    }
    printf("\n");
}

void analiseFrequencia(const char *txt) {
    int cont[ALFABETO] = {0}, total = 0;
    for (int i = 0; txt[i]; i++) {
        char c = txt[i];
        if (c >= 'A' && c <= 'Z') { cont[c - 'A']++; total++; }
    }
    EntradaFrequencia freq[ALFABETO];
    for (int i = 0; i < ALFABETO; i++) {
        freq[i].letra = 'A' + i;
        freq[i].contagem = cont[i];
        freq[i].frequencia = total ? (double)cont[i] / total * 100.0 : 0;
    }
    qsort(freq, ALFABETO, sizeof(EntradaFrequencia), comparaEntradasFrequencia);
    printf("\nAnalise de Frequencia\n");
    printf("+------------+-----------+--------------+-----------------+\n");
    printf("| Letra CT   | Cont. CT  | Freq CT   | Sugestao PT (F) |\n");
    printf("+------------+-----------+--------------+-----------------+\n");
    int idx = 0;
    for (int i = 0; i < ALFABETO; i++) {
        if (freq[i].contagem > 0) {
            printf("|    %c       |   %4d    | %6.2f%%   |     %c (%5.2f%%) |\n",
                freq[i].letra, freq[i].contagem, freq[i].frequencia,
                TABELA_WIKI[idx].letra, TABELA_WIKI[idx].freq_padrao);
            idx++;
        }
    }
    printf("+------------+-----------+--------------+-----------------+\n");
}

void criptografaArquivo(const char *in, const char *out, int shift) {
    FILE *fi = fopen(in, "r");
    FILE *fo = fopen(out, "w");
    if (!fi || !fo) {
        if (fi) fclose(fi);
        if (fo) fclose(fo);
        return;
    }
    int c;
    while ((c = fgetc(fi)) != EOF) {
        if (c >= 'A' && c <= 'Z') c = ((c - 'A' + shift) % 26) + 'A';
        else if (c >= 'a' && c <= 'z') c = ((c - 'a' + shift) % 26) + 'A';
        fputc(c, fo);
    }
    fclose(fi);
    fclose(fo);
}

void criptografaCorpus12(int shift) {
    const char *orig[12] = {
        "testes/Aglaea.txt","testes/Anaxa.txt","testes/Castorice.txt",
        "testes/Cerydra.txt","testes/Cipher.txt","testes/Cyrene.txt",
        "testes/Dang Heng - Terravox.txt","testes/Hyacine.txt",
        "testes/Hysilens.txt","testes/Mydei.txt",
        "testes/Phainon.txt","testes/Tribios.txt"
    };
    const char *out[12] = {
        "testes/Aglaea_cript.txt","testes/Anaxa_cript.txt","testes/Castorice_cript.txt",
        "testes/Cerydra_cript.txt","testes/Cipher_cript.txt","testes/Cyrene_cript.txt",
        "testes/Dang Heng - Terravox_cript.txt","testes/Hyacine_cript.txt",
        "testes/Hysilens_cript.txt","testes/Mydei_cript.txt",
        "testes/Phainon_cript.txt","testes/Tribios_cript.txt"
    };
    for (int i = 0; i < 12; i++) criptografaArquivo(orig[i], out[i], shift);
}

void analiseCorpus12() {
    const char *arqs[12] = {
        "testes/Aglaea_cript.txt","testes/Anaxa_cript.txt","testes/Castorice_cript.txt",
        "testes/Cerydra_cript.txt","testes/Cipher_cript.txt","testes/Cyrene_cript.txt",
        "testes/Dang Heng - Terravox_cript.txt","testes/Hyacine_cript.txt",
        "testes/Hysilens_cript.txt","testes/Mydei_cript.txt",
        "testes/Phainon_cript.txt","testes/Tribios_cript.txt"
    };
    int total[ALFABETO] = {0}, totalL = 0;
    for (int k = 0; k < 12; k++) {
        FILE *fp = fopen(arqs[k], "r");
        if (!fp) continue;
        int c;
        while ((c = fgetc(fp)) != EOF) {
            if (c >= 'A' && c <= 'Z') { total[c - 'A']++; totalL++; }
        }
        fclose(fp);
    }
    EntradaFrequencia f[ALFABETO];
    for (int i = 0; i < ALFABETO; i++) {
        f[i].letra = 'A' + i;
        f[i].contagem = total[i];
        f[i].frequencia = totalL ? (double)total[i] / totalL * 100.0 : 0;
    }
    qsort(f, ALFABETO, sizeof(EntradaFrequencia), comparaEntradasFrequencia);
    printf("\nAnalise de Frequencia com 12 Arquivos\n");
    printf("+------------+-----------+--------------+\n");
    printf("| Letra PT   | Contagem  | Freq (%%)     |\n");
    printf("+------------+-----------+--------------+\n");
    for (int i = 0; i < ALFABETO; i++) {
        if (f[i].contagem > 0)
            printf("|    %c       |   %5d   |   %6.2f%%   |\n",
                f[i].letra, f[i].contagem, f[i].frequencia);
    }
    printf("+------------+-----------+--------------+\n");
}

void export(const char *chave) {
    char nomeArq[100];
    char caminhoArq[200];
    FILE *file;
    printf("\nNome para salvar a CHAVE FINAL (sem extensao): ");
    if (scanf("%99s", nomeArq) != 1) return;
    snprintf(caminhoArq, sizeof(caminhoArq), "%s_chave_resultado.txt", nomeArq);
    file = fopen(caminhoArq, "w");
    if (!file) { printf("Erro ao criar arquivo.\n"); return; }
    fprintf(file, "Chave de Criptografia Final:\n");
    fprintf(file, "CT (Cripto): ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    fprintf(file, "PT (Plano) : ");
    for (int i = 0; i < ALFABETO; i++) fprintf(file, "%c", chave[i]);
    fprintf(file, "\n");
    fclose(file);
    printf("Chave salva em %s\n", caminhoArq);
}
