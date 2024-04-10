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

/* Não é necessário o argumento de iteração para as funções*/
unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}
unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

/* Primeiramente é necessário fazer um TAD de lista*/
typedef struct No {
    int chave;
    struct No *proximo;
}no;

typedef struct{
    no *inicio;
    no *fim;
    int tamanho;
}lista;

/* Alocação de memoria inicial e inicialização da struct */
lista *cria_lista()
{
    lista *l = (lista *) malloc(sizeof(lista));
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
    return l;
}

/* Função auxiliar de apagar todos os nós a partir de um começo */
void apaga_nos(no **comeco)
{
    no *atual = *comeco;
    no *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    
}
/* Desaloca a memoria de uma lista */
void apaga_lista(lista **l)
{
    apaga_nos(&((*l)->inicio));
    free((*l));
}
/* Insere um elemento na lista */
unsigned insere_lista(unsigned chave, lista *l)
{
    if (l == NULL) { // lista não inicializada
        return 0;
    }

    // Cria novo nó com a chave
    no *novo_no = (no *) malloc(sizeof(no));
    novo_no->proximo = NULL;
    novo_no->chave = chave;

    // Atualiza o ponteiro para o fim da lista
    if (l->fim != NULL) {
        l->fim->proximo = novo_no;
    }
    l->fim = novo_no;
    
    // Atualiza o ponteiro para o começo da lisa
    if (l->inicio == NULL) 
        l->inicio = novo_no;
    
    // Atualiza o tamanho da lista
    l->tamanho++;
    
    if (l->tamanho == 1)
        return 0; // não houve colisão

    return 1; // houve colisão
}

/* Procura uma chave a partir do nó inicial*/
unsigned busca_lista(int chave, lista *l)
{
    unsigned encontrados = 0;
    no *temp = l->inicio;
    while (temp != NULL) {
        if (temp->chave == chave)
            encontrados++;
        temp = temp->proximo;
    }
    return encontrados;
}

/* O TAD do hash é um vetor de listas */
typedef struct {
    lista **listas;
}hash;

/* Aloca memoria para o as listas do hash */
hash cria(unsigned B)
{
    hash tabela;
    tabela.listas = (lista **) malloc(B*sizeof(lista *));
    for (int i = 0; i < B; i++) {
        tabela.listas[i] = cria_lista();
    }
    return tabela;
}
/* Desaloca a memoria */
void apaga(hash tabela, unsigned B) {
    for (int i = 0; i < B; i++) {
        apaga_lista(&tabela.listas[i]);
    }
    free(tabela.listas);
}

/* O resto da tabela hash são simples, basta calcular a posição com 
 * a função hash e utilizar a função da lista nessa posição
 */
unsigned busca_div(unsigned chave, int B, hash tabela) {
    unsigned pos = h_div(chave, B);
    busca_lista(chave, tabela.listas[pos]);
}
unsigned insere_div(unsigned chave, int B, hash tabela) {
    unsigned pos = h_div(chave, B);
    insere_lista(chave, tabela.listas[pos]);
}

unsigned busca_mul(unsigned chave, int B, hash tabela) {
    unsigned pos = h_mul(chave, B);
    busca_lista(chave, tabela.listas[pos]);
}
unsigned insere_mul(unsigned chave, int B, hash tabela) {
    unsigned pos = h_mul(chave, B);
    insere_lista(chave, tabela.listas[pos]);
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);
    

    // cria tabela hash com hash por divisão
    hash tabela_div = cria(B);

    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        unsigned chave = converter(insercoes[i]);   
        colisoes_h_div += insere_div(chave, B, tabela_div);
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        unsigned chave = converter(consultas[i]);
        encontrados_h_div += busca_div(chave, B, tabela_div);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
    apaga(tabela_div, B);   

    // cria tabela hash com hash por multiplicação
    hash tabela_mul = cria(B);

    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        unsigned chave = converter(insercoes[i]); 
        colisoes_h_mul += insere_mul(chave, B, tabela_mul);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        unsigned chave = converter(consultas[i]); 
        encontrados_h_mul += busca_mul(chave, B, tabela_mul);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
    apaga(tabela_mul, B);

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

    // Desaloca a memoria   
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
