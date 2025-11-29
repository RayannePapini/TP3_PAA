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
const FreqPadrao TABELA_WIKI[ALFABETO] = {{'A',14.63},{'E',12.57},{'O',10.73},{'S',7.81},{'R',6.53},{'I',6.18},{'N',5.05},{'D',4.99},{'M',4.74},{'U',4.63},{'T',4.34},{'C',3.88},{'L',2.78},{'P',2.52},{'V',1.67},{'G',1.30},{'H',1.28},{'Q',1.20},{'B',1.04},{'F',1.02},{'Z',0.47},{'J',0.40},{'X',0.21},{'K',0.02},{'W',0.01},{'Y',0.01}
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
    for (int i = 0; i < ALFABETO; i++){
        chave[i] = DESCONHECIDO;
    } 
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

void exibeEstado(const char *cript, const char *chave) {
    int n = (int)strlen(cript);

    // Cria buffer do texto parcialmente decifrado
    char *decifrado = malloc(n + 1);
    if (!decifrado) {
        printf("Erro de alocacao de memoria.\n");
        return;
    }

    // Preenche o texto decifrado parcial
    for (int i = 0; i < n; i++) {
        char c = cript[i];
        if (c >= 'A' && c <= 'Z')
            decifrado[i] = (chave[c - 'A'] == DESCONHECIDO) ? c : chave[c - 'A'];
        else
            decifrado[i] = c;
    }
    decifrado[n] = '\0';

    printf("\n=== Estado Atual ===\n");
    printf("Texto Criptografado:\n%s\n", cript);

    printf("\n=== Chave Atual ===\n");
    printf("CT: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("PT: ");
    for (int i = 0; i < ALFABETO; i++) printf("%c", chave[i]);

    printf("\n\n=== Texto Parcialmente Decifrado ===\n");

    int inicio = 0;
    for (int i = 0; i <= n; i++) {

        // Detecta fim de linha
        if (cript[i] == '\n' || cript[i] == '\0') {

            //imprime a linha do texto decifrado
            for (int j = inicio; j < i; j++)
                putchar(decifrado[j]);
            putchar('\n');

            //imprime a linha dos '^' alinhados
            for (int j = inicio; j < i; j++) {
                char c = cript[j];
                if (c >= 'A' && c <= 'Z') {
                    if (chave[c - 'A'] != DESCONHECIDO)
                        putchar('^');
                    else
                        putchar(' ');
                } else {
                    putchar(' ');
                }
            }
            putchar('\n');

            inicio = i + 1; // nova linha
        }
    }

    free(decifrado);
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
    printf("| Letra CT   | Cont. CT  | Freq CT   | Sugestao PT (F) |\n");
    int idx = 0;
    for (int i = 0; i < ALFABETO; i++) {
        if (freq[i].contagem > 0) { // Exibe apenas letras que ocorrem no texto
            printf("|    %c      |   %4d    | %6.2f%%   |     %c (%5.2f%%) |\n",
                freq[i].letra, freq[i].contagem, freq[i].frequencia,
                TABELA_WIKI[idx].letra, TABELA_WIKI[idx].freq_padrao);
            idx++;
        }
    }
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
            // Criptografa letras maiúsculas
            c = ((c - 'A' + shift) % 26) + 'A';
        else if (c >= 'a' && c <= 'z') 
            // Converte minúscula para maiúscula e depois criptografa
            c = ((c - 'a' + shift) % 26) + 'A';
        fputc(c, fo); // Escreve o caractere criptografado
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
    printf("| Letra PT   | Contagem  | Freq (%%)     |\n"); 
    for (int i = 0; i < ALFABETO; i++) {
        if (f[i].contagem > 0)
            printf("|    %c      |   %5d   |   %6.2f%%   |\n",
                f[i].letra, f[i].contagem, f[i].frequencia);
    }
}

//exporta a chave de decifração final para um arquivo de texto
void export(const char *textoCriptografado, const char *chave){
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

            // escrevemos o texto decifrado no arquivo
            int tam = strlen(textoCriptografado);
            char *decifrado = malloc(tam + 1);
            decifraTexto(textoCriptografado, chave, decifrado);
            fprintf(file, "%s", decifrado);
            free(decifrado);
            printf("Seu arquivo foi criado e registrado com sucesso!\n");
            fclose(file);

            break;
        } else{
            printf("Houve um problema inexperado! Tente novamente\n");
            continue;
        }
    }

    printf("Resultado esperado no arquivo (decifrado):\n");
    {
        size_t n = strlen(textoCriptografado);
        char *decifrado = malloc(n + 1);
        if (decifrado) {
            decifraTexto(textoCriptografado, chave, decifrado);
            printf("%s\n", decifrado);
            free(decifrado);
        } else {
            printf("%s\n", textoCriptografado);
        }
    }
}