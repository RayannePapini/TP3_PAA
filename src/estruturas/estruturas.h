#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>

#define ALFABETO 26
#define DESCONHECIDO '_' 

#define COR_VERDE "\033[0;32m"
#define COR_RESET "\033[0m"
#define COR_VERMELHO "\033[0;31m"

extern int SHIFT_GLOBAL;

char *criptografia(char *texto);
char *geraArqCripto();
void menu(char *textoCriptografado);
void alteraChave(char chave[ALFABETO]);

void inicializaChave(char chave[ALFABETO]);
void decifraTexto(const char *texto_criptografado, const char *chave, char *texto_decifrado);
void exibeEstado(const char *texto_criptografado, const char *chave);
void analiseFrequencia(const char *texto_criptografado); 
void analiseCorpus12();

void altChave(char *textoCriptografado); 

void export(const char *chave); 

void realizarCasamentoExato(const char *textoCriptografado);

void criptografaCorpus12(int shift);

#endif
