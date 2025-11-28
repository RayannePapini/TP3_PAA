#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Constantes
#define ALFABETO 26
#define DESCONHECIDO '_' 

typedef struct {
    char letra;
    int contagem;
    double frequencia;
} EntradaFrequencia;

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

// Finalização/Exportação
void export(char *textoCriptografado, char *chave);

/* Casamento exato e aproximado */
void realizarCasamentoExato(const char *textoCriptografado, char chave[ALFABETO]);

#endif
