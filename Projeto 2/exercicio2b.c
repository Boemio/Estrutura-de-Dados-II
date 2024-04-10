// Vinícius de Moraes - 13749910
// Raul Ribeiro Teles - 13688176
// Bruno Ideriha Sugahara - 10310759

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
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


/* As funções hash foram alteradas para não receberem a 
 * iteração como argumento.
 */
unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return  (unsigned) (fmod(x * A, 1) * B);
}

/* A função h é o hash duplo, que utiliza as duas funções anteriores*/
unsigned h(unsigned x, unsigned i, unsigned B)
{
    return (h_mul(x, B) + i * h_div(x, B) ) % B;
}

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

/* Insere um elemento na tabela hash e retorna o número de colisões
 * a função assume que a tabela hash não está cheia
 */
unsigned insere(hash tab, unsigned chave, unsigned B)
{
    unsigned i;
    for (i = 0; i < B; i++) {
        unsigned pos = h(chave, i, B);
        if (tab.v[pos] < 0) { // posição livre
            tab.v[pos] = chave;
            break;
        }
    }
    return i;
}

/* Retorna quantos elementos tem determinada chave*/
unsigned busca(hash tab, unsigned chave, unsigned B)
{
    unsigned encontrados = 0;

    for (unsigned i = 0; i < B; i++) {
        unsigned pos = h(chave, i, B);
        if (tab.v[pos] == chave) {
            encontrados++;
        }
        else if (tab.v[pos] == -1) { // parada
            break;
        }
    }
    return encontrados;
}

/* Desaloca memoria da tabela hash*/
void limpa(hash h)
{
    free(h.v);
    h.v = NULL;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes = 0;
    unsigned encontrados = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    // cria tabela hash por hash duplo
    hash tab = cria(B);

    // inserção dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        unsigned chave = converter(insercoes[i]);
        colisoes += insere(tab, chave, B);
    }
    double tempo_insercao = finaliza_tempo();

    // busca dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        unsigned chave = converter(consultas[i]);
        encontrados += busca(tab, chave, B);

    }
    double tempo_busca = finaliza_tempo();

    // Desalocação da memoria da tabela
    limpa(tab);

    printf("Colisões na inserção: %d\n", colisoes);
    printf("Tempo de inserção   : %fs\n", tempo_insercao);
    printf("Tempo de busca      : %fs\n", tempo_busca);
    printf("Itens encontrados   : %d\n", encontrados);

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
