/*
 * Grupo:
 * Bruno Ideriha Sugahara - 10310759
 * Vinicius de Moraes - 13749910
 * Raul Ribeiro Teles - 13688176
 */
#include "lista.h"

/*
 * Aloca espaco para a lista
 * Inicializa o tamanho da lista como 0
 */
lista *criaLista() {
	lista* novaLista = malloc(sizeof(lista));
	novaLista->tamanho = 0;
	novaLista->elementos = NULL;
	return novaLista;
}

/* Desaloca a memoria da lista e de seu vetor de elementos */
void apagaLista(lista **lista) {
	free((*lista)->elementos);
	(*lista)->elementos = NULL;
	free(*lista);
	*lista = NULL;
}

/* Testa se a lista esta cheia ou vazia */
bool listaEstaVazia(lista *lista) {
	if(lista->tamanho == 0) {
		return true;
	}
	return false;
}
bool listaEstaCheia(lista *lista) {
	if(lista->tamanho == TAM_MAX) {
		return true;
	}
	return false;
}

/* 
 * Insere um elemento na lista
 * Caso ela esteja cheia, retorna falso
 */
bool insereLista(lista *lista, elemento elemento) {
	/* Verifica se a lista esta cheia */
	if (listaEstaCheia(lista)) {
		return false;
	}
	/* Insere o elemento */
	lista->tamanho++;
	lista->elementos = realloc(lista->elementos, lista->tamanho*sizeof(elemento)); /* Aloca mais memoria */
	lista->elementos[lista->tamanho - 1] = elemento;
	return true;
}

/* 
 * Imprime os elementos da lista para o stdout 
 * Imprime uma mensagem caso a lista esteja vazia
 */
void imprimeLista(lista *lista) {
	if (listaEstaVazia(lista)) {
		printf("Lista Vazia!\n");
		return;
	}
	/* Imprime a lista */
	for(long i = 0; i < lista->tamanho - 1; i++) {
		printf("%d ", lista->elementos[i]);
	}
	printf("%d\n", lista->elementos[lista->tamanho - 1]);
}

/* 
 * Troca dois elementos
 * Essa funcao recebe os poteiros para os elementos a serem trocados
 */
void troca(elemento *a, elemento *b) {
	if (a == b) return;
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

/* 
 * Essa funcao gera uma lista cujos elementos sao numeros aleatorios
 * Assume-se que foi gerada uma seed para os numeros aleatorios antes da chamada da funcao 
 */
lista *geraListaAleatoria(long tamanho) {
	lista *listaAleatoria = criaLista();

	for(int i = 0; i < tamanho; i++) {
		insereLista(listaAleatoria, rand()); 
	}

	return listaAleatoria;
}

//Gera uma lista aleatoria e ordena de forma crescente
lista *geraListaCrescente(long tamanho) {
	lista *lista = geraListaAleatoria(tamanho);
	quickSort(lista);
	return lista;
}

//Gera uma lista aleatoria e ordena de forma decrescente
lista *geraListaDecrescente(long tamanho) {
	lista *listaDecrescente = criaLista();
	lista *lista = geraListaCrescente(tamanho);
	for (int i = tamanho - 1; i >= 0; i--) {
		insereLista(listaDecrescente, lista->elementos[i]);
	}
	apagaLista(&lista);
	return listaDecrescente;
}

/*
 * Percorre o vetor n - 1 vezes
 * Em cada iteracao compara elementos lado a lado e os troca caso estejam desordenados
 * Na iteracao i, sabemos que os i maiores elementos estao ordenados no fim da lista
 */
void bubbleSort(lista *lista) {
	for (int i = 0; i < lista->tamanho - 1; i++) { // Percorre o vetor n - 1 vezes
		for (int j = 0; j < lista->tamanho - 1 - i; j++) { // Percorre cada elemento nao ordenado
			if (lista->elementos[j] > lista->elementos[j+1]) { // Compara o elemento com seu prox
				troca(&(lista->elementos[j]), &(lista->elementos[j+1]));
			}
		}
	}
}

/* 
 * Bubblesort com uma flag que testa se o vetor esta ordenado
 * A flag eh testada apos o vetor ter sido percorrido (primeiro for)
 */
void bubbleSortOtimizado(lista *lista) {
	for (int i = 0; i < lista->tamanho - 1; i++) {
		bool ordenado = true;
		for (int j = 0; j < lista->tamanho - 1 - i; j++) {
			if (lista->elementos[j] > lista->elementos[j+1]) {
				ordenado = false;	/* O vetor nao esta ordenado */
				troca(&(lista->elementos[j]), &(lista->elementos[j+1]));
			}
		}
		if (ordenado) {	/* Vetor ja esta ordenado */
			break;
		}
	}
}

/* Escolhe o ultimo elemento da lista como pivo e o coloca no lugar certo */
elemento particiona(lista *lista, long ini, long fim) {
	/* Escolha do pivo */
	elemento pivo = lista->elementos[fim];

	/* O i eh o indice onde o pivo vai ficar
	 * Para cada elemento menor ou igual ao pivo, esse elemento e colocado mais
	 * a esquerda do vetor, e o indice do pivo aumenta
	 * No fim do laco, todos os valores menores que o pivo estarao
	 * a esquerda de seu indice i 
	 */
	long i = ini;
	for (long j = ini; j < fim; j++) {
		if (lista->elementos[j] <= pivo) {
			troca(&(lista->elementos[i]), &(lista->elementos[j]));
        		i++;
        	}
    	}
	/* coloca o pivo na posicao correta e retorna a posicao do pivo */
	troca(&(lista->elementos[i]), &(lista->elementos[fim]));
	return i;
}

/* 
 * Escolhe um elemento entre [ini, fim] e o coloca no fim do vetor
 * Em seguida particiona o vetor utilizando o fim do vetor como pivo 
 */
elemento particaoAleatoria(lista *lista, long ini, long fim) {
	srand(clock()); 
    	long k = ini + rand() % (fim - ini); /* pivo escolhido de posicao aleatoria */
	troca(&(lista->elementos[k]), &(lista->elementos[fim])); /* Coloca o pivo escolhido no fim */
    	return particiona(lista, ini, fim); /*particiona o vetor de acordo com o pivo */
}

// Escolhe um pivo, particiona o vetor com ele e ordena o vetor repetindo esse processo recursivamente
void quickSortAux(lista *lista, long ini, long fim) {
	
	if (ini < fim) {
        	elemento pivo = particaoAleatoria(lista, ini, fim);
        	quickSortAux(lista, ini, pivo - 1);
        	quickSortAux(lista, pivo + 1, fim);
	}
}
void quickSort(lista *lista) {
	quickSortAux(lista, 0, lista->tamanho - 1);
}

// Retorna o maior elemento da lista
elemento pega_maior(lista *lista, long tamanho)
{
    elemento max = lista->elementos[0];

    for(long i = 0; i < tamanho; i++)
    {
        if(lista->elementos[i] > max)
        {
            max = lista->elementos[i];
        }
    }

    return max;
}

// Uso do counting sort como sub rotina
void counting_sort(lista *lista, long tamanho, int posicao)
{
    int b[10] = { 0 };
    int c[tamanho];
    
    for(int i = 0; i < tamanho; i++)
    {
        b[(lista->elementos[i]/posicao)%10]++;
    }
    
    for(int i = 1; i < 10; i++)
    {
        b[i] += b[i-1];
    }

    for(int i = tamanho - 1; i >= 0; i--)
    {
        c[b[(lista->elementos[i]/posicao)%10]-1] = lista->elementos[i];
        b[(lista->elementos[i]/posicao)%10]--;
    }

    for(int i = 0; i < tamanho; i++)
    {
        lista->elementos[i] = c[i];
    }
}

void radix_sort(lista *lista, long tamanho)
{
    elemento maior = pega_maior(lista,tamanho);
    int posicao = 1; 

    while(maior/posicao > 0)
    {
        counting_sort(lista,tamanho,posicao);
        posicao *= 10;
    }
}

/*
 * A funcao recebe uma arvore binaria que é "quase" uma heap
 * O unico elemento que pode fazer a arvore nao ser uma heap possui indice i
 * Essa funcao garante que a arvore eh uma heap
 */
void heapify(lista *lista, long n, long i) {
	//Inicializa os filhos de i e o maior entre i e seus filhos
	long esquerda, direita, maior;

	maior = i;
	esquerda = 2 * i + 1;
	direita = 2 * i + 2;
 
	//Descobrindo que indice entre esquerda, direita e i possui o maior valor
	if(esquerda < n && lista->elementos[esquerda] > lista->elementos[maior])
		maior = esquerda;
	if(direita < n && lista->elementos[direita] > lista->elementos[maior])
		maior = direita;
	if(maior != i) {
		troca(&(lista->elementos[i]), &(lista->elementos[maior]));	// Realiza a troca para satisfazer a propriedade pai > filho
		heapify(lista, n, maior); // Recursao para as arvores abaixo
	}
}

//Ordenacao utilizando a estrutura de dados heap
void heapSort(lista *lista, long n) {
	//Transforma a lista em uma heap
	for(long i = n / 2 - 1; i >= 0; i--) {
		heapify(lista, n , i);
	}
	/* 
	 * Troca a ultima folha com a raiz e rearranja a heap
	 * Temos que a raiz da arvore (o maior elemento da heap) eh colocado no fim da lista
	 * Assim temos que esse elemento retirado da heap esta ordenado na lista
	 * A heap eh rearranjada e o processo eh repetido ate toda a lista estar ordenada
	 */
	for(long i = n - 1; i >= 1; i--) {
		troca(&(lista->elementos[i]), &(lista->elementos[0]));
		heapify(lista, i, 0);
	}
}