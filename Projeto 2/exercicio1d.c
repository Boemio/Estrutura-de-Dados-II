// Vinícius de Moraes - 13749910
// Raul Ribeiro Teles - 13688176
// Bruno Ideriha Sugahara - 10310759

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int * inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
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

int main()
{
    const int N = 50000;
    unsigned encontrados = 0;
    int index[5][2];
    int aux;
    int j;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    // ordena o vetor entradas
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(entradas[j] > entradas[j+1])
            {
                aux = entradas[j];
                entradas[j] = entradas[j+1];
                entradas[j+1] = aux;
            }
        }
    }

    // cria o index
    index[0][0] = entradas[0];
    index[0][1] = 0;
    index[1][0] = entradas[9999];
    index[1][1] = 9999;
    index[2][0] = entradas[19999];
    index[2][1] = 19999;
    index[3][0] = entradas[29999];
    index[3][1] = 29999;
    index[4][0] = entradas[39999];
    index[4][1] = 39999;

    // realiza busca sequencial
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        if((consultas[i] > index[0][0]) && (consultas[i] < index[1][0]))
        {
            j = index[0][1];
        }
        if((consultas[i] > index[1][0]) && (consultas[i] < index[2][0]))
        {
            j = index[1][1];
        }
        if((consultas[i] > index[2][0]) && (consultas[i] < index[3][0]))
        {
            j = index[2][1];
        }
        if((consultas[i] > index[3][0]) && (consultas[i] < index[4][0]))
        {
            j = index[3][1];
        }
        if(consultas[i] > index[4][0])
        {
            j = index[4][1];
        }

        for(; j < N; j++)
        {
            if(consultas[i] == entradas[j])
            {
                encontrados++;
            }
        }
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
