#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>

#define ALFABETO 26 //letras de A-Z
#define DESCONHECIDO '_' //caracteres desconhecidos

extern int SHIFT_GLOBAL;

/*Funções para a criptografia*/
char *criptografia(char *texto);
char *geraArqCripto();
void criptografaCorpus12(int shift);

/*Funções para criptoanalise*/
void decifraTexto(const char *texto_criptografado, const char *chave, char *texto_decifrado);
void analiseFrequencia(const char *texto_criptografado);
void analiseCorpus12();
void realizarCasamentoExato(const char *textoCriptografado);
void realizarCasamentoAproximado(const char *textoCriptografado, const char *chave);

/*Funções para mecher com a chave*/
void inicializaChave(char chave[ALFABETO]);
void alteraChave(char chave[ALFABETO]);
void altChave(char *textoCriptografado);
void export(const char *chave);

/*Funções da interface*/
void menu(char *textoCriptografado);
void exibeEstado(const char *texto_criptografado, const char *chave);

#endif

