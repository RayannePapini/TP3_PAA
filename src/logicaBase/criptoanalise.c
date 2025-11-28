#include "../estruturas/estruturas.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tipo de estrutura para armazenar a frequência padrão das letras
typedef struct { 
    char letra;         
    double freq_padrao; 
} FreqPadrao;

// Tabela de frequência padrão das letras na língua portuguesa (baseada na Wikipedia)
const FreqPadrao TABELA_WIKI[ALFABETO] = {
    {'A',14.63},{'E',12.57},{'O',10.73},{'S',7.81},{'R',6.53},
    {'I',6.18},{'N',5.05},{'D',4.99},{'M',4.74},{'U',4.63},
    {'T',4.34},{'C',3.88},{'L',2.78},{'P',2.52},{'V',1.67},
    {'G',1.30},{'H',1.28},{'Q',1.20},{'B',1.04},{'F',1.02},
    {'Z',0.47},{'J',0.40},{'X',0.21},{'K',0.02},{'W',0.01},
    {'Y',0.01}
};

// Tipo de estrutura para armazenar a frequência de letras do texto cifrado
typedef struct { 
    char letra;      
    int contagem;    
    double frequencia; 
} EntradaFrequencia;

// Função de comparação para qsort, usada para ordenar as entradas de frequência 
int comparaEntradasFrequencia(const void *a, const void *b) {
    const EntradaFrequencia *A = a;
    const EntradaFrequencia *B = b;
    return B->contagem - A->contagem;
}

// Inicializa a chave de decifração com o valor DESCONHECIDO para todas as letras
void inicializaChave(char chave[ALFABETO]) {
    for (int i = 0; i < ALFABETO; i++) chave[i] = DESCONHECIDO;
}

// Decifra o texto criptografado usando a chave atual.
void decifraTexto(const char *cript, const char *chave, char *decifrado) {
    int n = (int)strlen(cript);
    for (int i = 0; i < n; i++) {
        char c = cript[i];
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            // Se a letra correspondente na chave for conhecida, usa a decifrada; senão, mantém a criptografada.
            decifrado[i] = (chave[idx] == DESCONHECIDO) ? c : chave[idx];
        } else {
            // Não é uma letra, mantém o caractere original
            decifrado[i] = c;
        }
    }
    decifrado[n] = '\0'; 
}

// Exibe o estado atual da criptanálise
void exibeEstado(const char *cript, const char *chave) {
    printf("\nEstado Atual\n");
    printf("Texto Criptografado:\n%s\n", cript);
    printf("\nChave Atual:\n");
    printf("CT: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("PT: ");
    // Exibe a chave atual (a substituição de cada letra cifrada 'CT' para a letra plana 'PT')
    for (int i = 0; i < ALFABETO; i++) printf("%c", chave[i]);
    printf("\n\nTexto Parcialmente Decifrado:\n");
    
    // Exibe o texto parcialmente decifrado, colorindo as letras decifradas
    int n = (int)strlen(cript);
    for (int i = 0; i < n; i++) {
        char c = cript[i];
        if (c >= 'A' && c <= 'Z') {
            // Se a chave for conhecida, imprime em verde; senão, em vermelho
            if (chave[c - 'A'] != DESCONHECIDO)
                printf("%s%c%s", COR_VERDE, chave[c - 'A'], COR_RESET);
            else
                printf("%s%c%s", COR_VERMELHO, c, COR_RESET);
        } else printf("%c", c); // Não é letra, imprime normal
    }
    printf("\n");
}

// Realiza a análise de frequência do texto cifrado e sugere correspondências baseadas na frequência padrão
void analiseFrequencia(const char *txt) {
    int cont[ALFABETO] = {0}, total = 0;
    
    // conta as ocorrências de cada letra maiúscula no texto
    for (int i = 0; txt[i]; i++) {
        char c = txt[i];
        if (c >= 'A' && c <= 'Z') { cont[c - 'A']++; total++; }
    }
    
    // preenche a estrutura EntradaFrequencia
    EntradaFrequencia freq[ALFABETO];
    for (int i = 0; i < ALFABETO; i++) {
        freq[i].letra = 'A' + i;
        freq[i].contagem = cont[i];
        // Calcula a frequência percentual
        freq[i].frequencia = total ? (double)cont[i] / total * 100.0 : 0;
    }
    
    // ordena as letras do texto cifrado por frequência decrescente
    qsort(freq, ALFABETO, sizeof(EntradaFrequencia), comparaEntradasFrequencia);
    
    //exibe a tabela de análise de frequência e as sugestões
    printf("\nAnalise de Frequencia\n");
    printf("+------------+-----------+--------------+-----------------+\n");
    printf("| Letra CT   | Cont. CT  | Freq CT   | Sugestao PT (F) |\n");
    printf("+------------+-----------+--------------+-----------------+\n");
    int idx = 0;
    for (int i = 0; i < ALFABETO; i++) {
        if (freq[i].contagem > 0) { // Exibe apenas letras que ocorrem no texto
            printf("|    %c      |   %4d    | %6.2f%%   |     %c (%5.2f%%) |\n",
                freq[i].letra, freq[i].contagem, freq[i].frequencia,
                TABELA_WIKI[idx].letra, TABELA_WIKI[idx].freq_padrao);
            idx++;
        }
    }
    printf("+------------+-----------+--------------+-----------------+\n");
}

// Criptografa um arquivo de entrada para um arquivo de saída usando a Cifra de César com 'shift'
void criptografaArquivo(const char *in, const char *out, int shift) {
    FILE *fi = fopen(in, "r");
    FILE *fo = fopen(out, "w");
    if (!fi || !fo) { // Verifica erros de abertura
        if (fi) fclose(fi);
        if (fo) fclose(fo);
        return;
    }
    int c;
    while ((c = fgetc(fi)) != EOF) {
        if (c >= 'A' && c <= 'Z') 
            // Criptografa letras maiúsculas: ((c - 'A' + shift) % 26) + 'A'
            c = ((c - 'A' + shift) % 26) + 'A';
        else if (c >= 'a' && c <= 'z') 
            // Converte minúscula para maiúscula e depois criptografa
            c = ((c - 'a' + shift) % 26) + 'A';
        fputc(c, fo); // Escreve o caractere criptografado/preservado
    }
    fclose(fi);
    fclose(fo);
}

// Criptografa um conjunto predefinido de 12 arquivos (corpus) com a Cifra de César
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
    // Chama a função de criptografia para cada par de arquivos
    for (int i = 0; i < 12; i++) criptografaArquivo(orig[i], out[i], shift);
}

// Realiza a análise de frequência em um corpus de 12 arquivos (os arquivos criptografados)
void analiseCorpus12() {
    const char *arqs[12] = {
        "testes/Aglaea_cript.txt","testes/Anaxa_cript.txt","testes/Castorice_cript.txt",
        "testes/Cerydra_cript.txt","testes/Cipher_cript.txt","testes/Cyrene_cript.txt",
        "testes/Dang Heng - Terravox_cript.txt","testes/Hyacine_cript.txt",
        "testes/Hysilens_cript.txt","testes/Mydei_cript.txt",
        "testes/Phainon_cript.txt","testes/Tribios_cript.txt"
    };
    int total[ALFABETO] = {0}, totalL = 0; // Contadores de frequência e total de letras
    
    // processa todos os arquivos, acumulando a contagem de letras
    for (int k = 0; k < 12; k++) {
        FILE *fp = fopen(arqs[k], "r");
        if (!fp) continue; // Pula se não conseguir abrir
        int c;
        while ((c = fgetc(fp)) != EOF) {
            // Conta apenas letras maiúsculas
            if (c >= 'A' && c <= 'Z') { total[c - 'A']++; totalL++; }
        }
        fclose(fp);
    }
    
    //calcula as frequências
    EntradaFrequencia f[ALFABETO];
    for (int i = 0; i < ALFABETO; i++) {
        f[i].letra = 'A' + i;
        f[i].contagem = total[i];
        f[i].frequencia = totalL ? (double)total[i] / totalL * 100.0 : 0;
    }
    
    //ordena as letras por frequência decrescente
    qsort(f, ALFABETO, sizeof(EntradaFrequencia), comparaEntradasFrequencia);
    
    //exibe a tabela de frequência do corpus
    printf("\nAnalise de Frequencia com 12 Arquivos\n");
    printf("+------------+-----------+--------------+\n");
    printf("| Letra PT   | Contagem  | Freq (%%)     |\n"); 
    printf("+------------+-----------+--------------+\n");
    for (int i = 0; i < ALFABETO; i++) {
        if (f[i].contagem > 0)
            printf("|    %c      |   %5d   |   %6.2f%%   |\n",
                f[i].letra, f[i].contagem, f[i].frequencia);
    }
    printf("+------------+-----------+--------------+\n");
}

//exporta a chave de decifração final para um arquivo de texto
void export(const char *chave) {
    char nomeArq[100];
    char caminhoArq[200];
    FILE *file;
    
    //pede ao usuário o nome do arquivo
    printf("\nNome para salvar a CHAVE FINAL (sem extensao): ");
    if (scanf("%99s", nomeArq) != 1) return;
    
    //cria o nome de arquivo completo 
    snprintf(caminhoArq, sizeof(caminhoArq), "%s_chave_resultado.txt", nomeArq);
    
    // Abre o arquivo para escrita
    file = fopen(caminhoArq, "w");
    if (!file) { printf("Erro ao criar arquivo.\n"); return; }
    
    // Escreve a chave no formato CT -> PT
    fprintf(file, "Chave de Criptografia Final:\n");
    fprintf(file, "CT (Cripto): ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    fprintf(file, "PT (Plano) : ");
    for (int i = 0; i < ALFABETO; i++) fprintf(file, "%c", chave[i]);
    fprintf(file, "\n");
    
    fclose(file);
    printf("Chave salva em %s\n", caminhoArq);
}
