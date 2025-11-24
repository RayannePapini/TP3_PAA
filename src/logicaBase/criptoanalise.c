#include "../estruturas/estruturas.h"
#include <stdio.h>
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

void export(){

}