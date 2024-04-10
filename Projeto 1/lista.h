/*
 * Grupo:
 * Bruno Ideriha Sugahara - 10310759
 * Vinicius de Moraes - 13749910
 * Raul Ribeiro Teles - 13688176
 */
#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define TAM_MAX 100001

typedef int elemento;

typedef struct lista_t {
	elemento *elementos;
	long tamanho;
}lista;

lista *criaLista();
void apagaLista(lista **lista);
void imprimeLista(lista *lista);
bool insereLista(lista *lista, elemento elemento);

bool listaEstaCheia(lista *lista);
bool listaEstaVazia(lista *lista);

/* Funcoes auxiliares dos vetores */
lista *geraListaAleatoria(long tamanho);
lista *geraListaCrescente(long tamanho);
lista *geraListaDecrescente(long tamanho);

/* Funcoes auxiliares dos algoritmos de ordenacao */
void troca(elemento *a, elemento *b);
elemento retornaMaior(elemento a, elemento b);
elemento retornaMenor(elemento a, elemento b);

/* Algoritmos de ordenacao */
//Bubblesort
void bubbleSort(lista *lista);
void bubbleSortOtimizado(lista *lista);
//Quicksort
void quickSort (lista *lista);
/*void quicksort (lista *lista, long ini, long fim);
elemento random_partition (lista *lista, long ini, long fim);
elemento partition (lista *lista, long ini, long fim);*/
//Radix Sort
void radix_sort(lista *lista, long tamanho);
void counting_sort(lista *lista, long tamanho, int posicao);
elemento pega_maior(lista *lista, long tamanho);
//Heapsort
void heapSort(lista *lista, long n);
void heapify(lista *lista, long n, long i);

#endif
