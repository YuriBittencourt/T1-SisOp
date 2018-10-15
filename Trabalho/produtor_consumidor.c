#include <stdio.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>
#include <stdlib.h>
#include "filter_lock.h"

#define N_PRODUCERS	3
#define N_CONSUMERS	1
#define N_BUFFER 6

Filter mutex;

sem_t empty, full;

// ----------------------
// inicio parte do buffer
// ----------------------
int buffer[N_BUFFER];
int insertCountBuffer = -1;
int removeCountBuffer = 0;
int itemCountBuffer = 0;

// inserir um elemento no buffer
void enter_item (int data){
	if (itemCountBuffer != N_BUFFER){
		// reinicia o buffer colocando o contador
		// na primeira posicao
		if (insertCountBuffer == N_BUFFER-1){
			insertCountBuffer = -1;
		}
		buffer[++insertCountBuffer] = data;
		itemCountBuffer++;
	}
}

// remove um elemento do buffer
int remove_item(){
	int data = buffer[removeCountBuffer++];
	// se foi colocado o ultimo elemento
	// deve-se reiniciar o buffer
	if (removeCountBuffer == N_BUFFER){
		removeCountBuffer = 0;
	}

	itemCountBuffer--;
	return data;

}
// ----------------------
// fim parte do buffer
// ----------------------

void *producer(void *arg){
	srand(time(NULL)); 
	int id = *(int*)arg;
	while(1){
		// gera um numero aleatorio entre 0-65
		int item = rand()  % (65 + 1 - 0) + 0;
		sem_wait(&empty); // decrementa o contador vazio 
						  //ideia de estar produzindo algo
		filter_lock(&mutex,id);
		printf("Produtor %d produzindo o item %d\n",id,item);
		enter_item(item);
		filter_unlock(&mutex,id);
		sem_post(&full); // incrementa o slot full, dizendo que
						 // algo foi produzido
	}
}

void *consumer(void *arg){
	int id = *(int*)arg;
	while (1){
		sem_wait(&full);	// decrementa o contador full 
						    //ideia de estar consumindo algo
		filter_lock(&mutex,id);
		int data = remove_item();
		printf("consumidor %d consumindo item %d\n", id, data);
		filter_unlock(&mutex,id);
		sem_post(&empty); // incrementa o slot empty, dizendo que
						 // algo foi consumido
	}
}

void main (){

	int size_threads = N_PRODUCERS + N_CONSUMERS;


	pthread_t producers[N_PRODUCERS], consumers[N_CONSUMERS];
	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));

	sem_init(&empty,0,N_BUFFER);
	sem_init(&full,0,0);

	init_filter_lock(&mutex,size_threads);

	int i;

	// criacao das threads dos consumidores
	for (i = 0; i < N_CONSUMERS; i++){
		array_threads_ids[i] = i;
		pthread_create(&consumers[i],NULL,consumer,(void*)&array_threads_ids[i]);
	}

	// para inicializar o vetor de ids para 
	// os escritores a partir da posicao N_CONSUMERS-1
	int cont = N_CONSUMERS-1;


	// criacao das threads dos produtores
	for (i = 0; i < N_PRODUCERS; i++){
		array_threads_ids[cont] = cont;
		pthread_create(&producers[i],NULL,producer,(void*)&array_threads_ids[cont]);
		cont++;
	}

	pthread_exit(NULL); 
}