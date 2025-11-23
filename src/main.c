#include <stdio.h>
#include "estruturas/estruturas.h"

int main(){

    char *textoCriptografado = geraArqCripto();
    printf("%s\n", textoCriptografado);

    return 0;
}