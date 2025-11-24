#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void altChave(char *textoCriptografado){
    char original, mapeada;
    printf("\nDigite nesta ordem separadoo por espaço:\n");
    printf("Letra original -> Letra mapeada\n");
    printf("Exemplo: A S\n");
    scanf("> %c %c", &original, &mapeada);
    original = toupper(original);
    mapeada = toupper(mapeada);

    // atualizando os valores no texto criptografado
    int tamTxt = strlen(textoCriptografado);
    for(int i=0; i<tamTxt; i++){
        if(textoCriptografado[i] == mapeada){
            textoCriptografado[i] = original;
        }
    }

    printf("Registrado: %c -> %c!\n", original, mapeada);
}

void export(char *textoCriptografado){
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
            fprintf(file, textoCriptografado);
            printf("Seu arquivo foi criado e registrado com sucesso!\n");
            fclose(file);

            break;
        } else{
            printf("Houve um problema inexperado! Tente novamente\n");
            continue;
        }
    }
}