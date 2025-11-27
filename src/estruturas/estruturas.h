#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Constantes
#define ALFABETO 26
#define DESCONHECIDO '_' 

/* Menu/Arquivo */
char *criptografia(char *texto);
char *geraArqCripto();
void menu(char *textoCriptografado);
void alteraChave(char chave[ALFABETO]);

/* Criptoanálise */
void inicializaChave(char chave[ALFABETO]);
void decifraTexto(const char *texto_criptografado, const char *chave, char *texto_decifrado);
void exibeEstado(const char *texto_criptografado, const char *chave);
void analiseFrequencia(const char *texto_criptografado, char chave[ALFABETO]);
void realizarCasamentoExato(const char *textoCriptografado, char chave[ALFABETO]);

/* Casamento exato e aproximado */

#endif