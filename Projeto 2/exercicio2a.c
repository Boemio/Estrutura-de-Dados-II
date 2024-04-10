// Vinícius de Moraes - 13749910
// Raul Ribeiro Teles - 13688176
// Bruno Ideriha Sugahara - 10310759

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

/* A tabela hash não é nada mais que um vetor de int*/
typedef struct {
    int *v;
}hash;

/* Aloca a memoria para o vetor e o inicializa com -1 */
hash cria(unsigned tamanho)
{
    hash h;
    h.v = malloc(tamanho * sizeof(int));
    
    /* inicializa o vetor com -1*/
    for (int i = 0; i < tamanho; i++)
    {
        h.v[i] = -1;
    }

    return h;
}

/* Insere um elemento em um hash com funcao hash de divisao
 * Retorna o numero de colisoes
 * A função assume que a tabela hash não está cheia
 */
unsigned insere_div(hash tab, unsigned chave, unsigned B)
{
    unsigned i; // i é a iteração
    for (i = 0; i < B; i++) {
        unsigned pos = h_div(chave, i, B);
        if (tab.v[pos] < 0) { // a posição está livre
            tab.v[pos] = chave;
            break;
        }
    }

    return i; // i também representa o número de colisões
}

/* Busca por uma chave e retorna quantos elementos a possuem
 * É procurado até achar um -1, que representa q essa parte do 
 * vetor nunca foi alcançado
 */
unsigned busca_div(hash tab, unsigned chave, unsigned B)
{
    unsigned encontrados = 0;

    for (unsigned i = 0; i < B; i++) {
        unsigned pos = h_div(chave, i, B); 
        if (tab.v[pos] == chave) {
            encontrados++;
        }
        else if (tab.v[pos] == -1) { // parada
            break;
        }
    }
    return encontrados;
}


/* As próximas duas funções para o hash de multiplicação são análogas 
 * às duas anteriores para o hash de divisão 
 */
unsigned insere_mul(hash tab, unsigned chave, unsigned B)
{
    unsigned i;
    for (i = 0; i < B; i++) {
        unsigned pos = h_mul(chave, i, B);
        if (tab.v[pos] < 0) {
            tab.v[pos] = chave;
            break;
        }
    }

    return i;
}
int busca_mul(hash tab, unsigned chave, unsigned B)
{
    unsigned encontrados = 0;

    for (unsigned i = 0; i < B; i++) {
        unsigned pos = h_mul(chave, i, B);
        if (tab.v[pos] == chave) {
            encontrados++;
        }
        else if (tab.v[pos] == -1) {
            break;
        }
    }
    return encontrados;
}

/* Desaloca memoria do hash*/
void limpa(hash h)
{
    free(h.v);
    h.v = NULL;
}


int main(int argc, char const *argv[])
{
    // Constantes e contadores
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    // cria tabela hash com hash por divisão
    hash tab_div = cria(B);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        unsigned chave = converter(insercoes[i]);
        colisoes_h_div += insere_div(tab_div, chave, B);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        unsigned chave = converter(consultas[i]);
        encontrados_h_div += busca_div(tab_div, chave, B);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão
    limpa(tab_div);

    // cria tabela hash com hash por multiplicação
    hash tab_mul = cria(B);

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        unsigned chave = converter(insercoes[i]);
        colisoes_h_mul += insere_mul(tab_mul, chave, B);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        unsigned chave = converter(consultas[i]);
        encontrados_h_mul += busca_mul(tab_mul, chave, B);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação
    limpa(tab_mul);


    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    for (int i = 0; i < N; i++) {
        free(insercoes[i]);
    }
    free(insercoes);
    for (int i = 0; i < M; i++) {
        free(consultas[i]);
    }
    free(consultas);

    return 0;
}
