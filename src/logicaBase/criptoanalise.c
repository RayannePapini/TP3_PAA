#include "../estruturas/estruturas.h"
#include <stdio.h>
#include <ctype.h>

void altChave(){
    char original, mapeada;
    printf("\nDigite nesta ordem separadoo por espaço:\n");
    printf("Letra original -> Letra mapeada\n");
    printf("Exemplo: A S\n");
    scanf("> %c %c", &original, &mapeada);
    original = toupper(original);
    mapeada = toupper(mapeada);

    // falta o principal que é registrar

    printf("Registrado: %c -> %c!", original, mapeada);
}