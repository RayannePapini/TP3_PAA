#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 

void alteraChave(char chave[ALFABETO]) { 
    char entradaStr[3];
    char original, mapeada;

    printf("\nInforme a letra original (Plaintext) e a letra criptografada (Ciphertext) para mapear (Ex: AS): ");

    if (scanf("%2s", entradaStr) != 1 || strlen(entradaStr) != 2 || !isalpha(entradaStr[0]) || !isalpha(entradaStr[1])) {
        printf("Formato invalido. Insira duas letras (Original -> Criptografada).\n");
        int c; 
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    original = toupper(entradaStr[0]);
    mapeada = toupper(entradaStr[1]);

    int indiceCT = mapeada - 'A';

    if (indiceCT >= 0 && indiceCT < ALFABETO) {
        for (int i = 0; i < ALFABETO; i++) {
            if (chave[i] == original && i != indiceCT) {
                printf("ERRO: A letra '%c' (Plaintext) ja esta mapeada para '%c' (Ciphertext).\n", original, 'A' + i);
                return;
            }
        }

        chave[indiceCT] = original;
        printf("Registrado: %c (PT) -> %c (CT).\n", original, mapeada);
    } else {
        printf("Letras invalidas ou fora do alfabeto.\n");
    }
}

char *criptografia(char *texto){
    char alph[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char cripto[26];
    // definido chave X com valor 3
    int X = 3;
    
    // montando vetor com valores criptografados
    for(int i=0; i<26; i++){
        int calculoCifDesc = (i+X)%26;
        cripto[i] = alph[calculoCifDesc];
    }

    // criptografando a partir do texto passado como parâmetro
    int tamTxt = strlen(texto);
    char *resultCripto = (char*) malloc(tamTxt + 1);
    int posCripto = 0;

    for(int i=0; i<tamTxt; i++){
        int encontrado = 0;

        for(int j=0; j<26; j++){
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
                export(textoCriptografado);
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
