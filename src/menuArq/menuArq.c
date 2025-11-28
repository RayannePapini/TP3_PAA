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
char *criptografia(char *texto){
    char alph[ALFABETO] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char cripto[ALFABETO];
    // definido chave X com valor 3
    int X = 3;
    
    // montando vetor com valores criptografados
    for(int i=0; i<ALFABETO; i++){
        int calculoCifDesc = (i+X)%ALFABETO;
        cripto[i] = alph[calculoCifDesc];
    }

    // criptografando a partir do texto passado como parâmetro
    int tamTxt = strlen(texto);
    char *resultCripto = (char*) malloc(tamTxt + 1);
    int posCripto = 0;

    for(int i=0; i<tamTxt; i++){
        int encontrado = 0;

        for(int j=0; j<ALFABETO; j++){
            if(texto[i] == alph[j]){
                // escreve em resultCripto de acordo com o valor do índice correspondente entre alph e cripto
                resultCripto[posCripto] = cripto[j];
                posCripto++;
                encontrado = 1;
                // achou caractere
                break;
            }
        }

        if(encontrado == 0){
            // Não encontrou no alfabeto logo é um espaço
            resultCripto[posCripto] = texto[i];
            posCripto++;
        }
    }

    // garante que string está fechada
    resultCripto[posCripto] = '\0';

    return resultCripto;
}

char *geraArqCripto(){
    char *txtUpper = NULL;
    char *txtCriptografado = NULL;

    while(1){
        // carregando arquivo de acordo com entrada do usuário
        printf("\nDigite o nome do arquivo de entrada para criptografia(sem extensao):\n");
        printf("O arquivo criptografado gerado sera usado na criptoanalise!\n");
        char nomeArq[100];
        scanf("%s", nomeArq);
        char caminhoArq[150];
        sprintf(caminhoArq, "testes/%s.txt", nomeArq);

        FILE *file = fopen(caminhoArq, "r");
        if(file == NULL){
            printf("Erro ao abrir o arquivo! Tente novamente\n");
            continue;
        }

        // encontrando tamanho do arquivo
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        //volta file pro início para a leitura
        fseek(file, 0, SEEK_SET);

        //aloca espaço necessário para o texto upper
        txtUpper = (char*) malloc(fileSize + 1);

        int ch; // int necessário para retornar EOF
        int pos=0;
        while ((ch = fgetc(file)) != EOF) {
            txtUpper[pos] = (char) toupper(ch);
            pos++;
        }
        
        // garante que string está fechada
        txtUpper[pos] = '\0';

        // criptografar txtUpper
        txtCriptografado = criptografia(txtUpper);

        fclose(file);
        // break é chamado para terminar o loop já que o arquivo lido é válido
        break;
    }
    return txtCriptografado;
}

void menu(char *textoCriptografado){
    char chave_decifracao[ALFABETO];
    inicializaChave(chave_decifracao);

    while(1){
        printf("\nDigite o numero corresponde ao que deseja:\n");
        printf("1. Apresentar o estado atual da criptoanalise\n");
        printf("2. Fazer um chute baseado analise de frequencia no texto criptografado\n");
        printf("3. Realizar casamento exato de caracteres no texto criptografado\n");
        printf("4. Realizar casamento aproximado de caracteres no texto parcialmente decifrado\n");
        printf("5. Alterar chave de criptografia\n");
        printf("6. Exportar resultado e encerrar o programa\n");
        int opcao=0;

        if (scanf("%d", &opcao) != 1) {
            printf("\nOpcao invalida. Por favor, insira um numero.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            continue;
        }

        int c; 
        while ((c = getchar()) != '\n' && c != EOF);

        switch (opcao){
            case 1:
                exibeEstado(textoCriptografado, chave_decifracao);
                break;
            case 2:
                analiseFrequencia(textoCriptografado, chave_decifracao);
                break;
            case 3:
                realizarCasamentoExato(textoCriptografado, chave_decifracao);
                break;
            case 4:
                printf("\nOpcao 4: Casamento Aproximado (a ser implementado)\n");
                break;
            case 5:
                alteraChave(chave_decifracao);
                break;
            case 6:
                export(textoCriptografado, chave_decifracao);
                break;        
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                continue;
        }

        if(opcao == 6){
            break;
        }
    }

}
