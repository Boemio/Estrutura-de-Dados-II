/*
 * Grupo:
 * Bruno Ideriha Sugahara - 10310759
 * Vinicius de Moraes - 13749910
 * Raul Ribeiro Teles - 13688176
 */
#include "lista.h"
#include <math.h>
#define REP 10
#define POTENCIA_MIN 3
#define POTENCIA_MAX 5

int main() {
	srand(clock()); /* Seed para numeros aleatorios */
	int nOrdensMagnitude = POTENCIA_MAX - POTENCIA_MIN + 1;
	/* Cria um vetor aleatorio de tamanho n e calcula o tempo que o algoritmo leva para ordena-lo 
	 * Repete o processo REP vezes para evitar ruido
	 * Repete o processo para diferentes valores de n para vez como a funcao cresce
	 * Tudo eh guardado no vetor medicoes
	 */
	double medicoes[nOrdensMagnitude][REP];
	for (int i = POTENCIA_MIN; i <= POTENCIA_MAX; i++) { /* varia o n */
		int n = pow(10,i);
		for(int j = 0; j < REP; j++) /* Repete o calculo REP vezes */{
			lista *lista = geraListaAleatoria(n);
			//lista *lista = geraListaCrescente(n);
			//lista *lista = geraListaDecrescente(n);
			clock_t clockIni = clock();
			//bubbleSort(lista);
			//bubbleSortOtimizado(lista);
			//quickSort(lista);
			//radix_sort(lista, n);
			heapSort(lista, n);
			long tempoExec = clock() - clockIni; /* Tempo levado em nmr de clocks */
			medicoes[i-POTENCIA_MIN][j] = (float)tempoExec/CLOCKS_PER_SEC; /* converte o resultado para segundos e o guarda */
			apagaLista(&lista);
		}
	}	

	/* Calcula media */
	/* Inicializa o vetor media com 0's */
	double medias[nOrdensMagnitude];
	for (int i = 0; i< nOrdensMagnitude; i++)
		medias[i] = 0;
	for (int i = 0; i < nOrdensMagnitude; i++) {
		for (int j = 0; j < REP; j++) {
			medias[i] += medicoes[i][j];
		}
		medias[i] /= REP;
	}
	/* Imprime o resultado */
	printf("== Medias ===\n");
	for (int i = 0; i < nOrdensMagnitude; i++)
		printf("n = %ld \t%lf\n", (long) pow(10,i+POTENCIA_MIN), medias[i]);

	/* Inicializa o vetor variancia como 0's */
	double variancia[nOrdensMagnitude];
	for (int i = 0; i< nOrdensMagnitude; i++)
		variancia[i] = 0;
	/* Calcula a variancia */
	for (int i = 0; i < nOrdensMagnitude; i++)
	{
		for (int j = 0; j < REP; j++) {
			variancia[i] += pow(medicoes[i][j] - medias[i], 2);
		}
		variancia[i] /= REP - 1;
	}

	/* Calcula o desvio padrao e imprime*/
	double desvioPadrao[nOrdensMagnitude];
	printf("== Desvios Padroes ==\n");
	for(int i = 0; i < nOrdensMagnitude; i++) {
		desvioPadrao[i] = sqrt(variancia[i]);
		printf("n = %ld \t%lf\n", (long) pow(10,i+POTENCIA_MIN), desvioPadrao[i]);
	}	


	return 0;
}
