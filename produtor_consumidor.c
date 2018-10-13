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

int buffer[N_BUFFER];
int insertCountBuffer = -1;
int removeCountBuffer = 0;
int itemCountBuffer = 0;

void enter_item (int data){
	if (itemCountBuffer != N_BUFFER){
		if (insertCountBuffer == N_BUFFER-1){
			insertCountBuffer = -1;
		}
		buffer[++insertCountBuffer] = data;
		itemCountBuffer++;
	}
}

int remove_item(){
	int data = buffer[removeCountBuffer++];
	if (removeCountBuffer == N_BUFFER){
		removeCountBuffer = 0;
	}

	itemCountBuffer--;
	return data;

}


void *producer(void *arg){
	srand(time(NULL)); 
	int id = *(int*)arg;
	while(1){
		int item = rand()  % (65 + 1 - 0) + 0;
		//printf("Produtor %d produzindo o item %d\n",id,item);
		sem_wait(&empty);
		//sem_wait(&mutex);
		filter_lock(&mutex,id);
		enter_item(item);
		filter_unlock(&mutex,id);
		//sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(void *arg){
	int id = *(int*)arg;
	while (1){
		sem_wait(&full);
		//sem_wait(&mutex);
		filter_lock(&mutex,id);
		int data = remove_item();
		filter_unlock(&mutex,id);
		//sem_post(&mutex);
		sem_post(&empty);
		//printf("consumidor %d consumindo item %d\n", id, data);
	}
}

void main (){

	int size_threads = N_PRODUCERS + N_CONSUMERS;

	//sem_init(&mutex,0,1);
	sem_init(&empty,0,N_BUFFER);
	sem_init(&full,0,0);

	pthread_t producers[N_PRODUCERS], consumers[N_CONSUMERS];

	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));

	init_filter_lock(&mutex,size_threads);

	int i;
	for (i = 0; i < N_CONSUMERS; i++){
		array_threads_ids[i] = i;
		pthread_create(&consumers[i],NULL,consumer,(void*)&array_threads_ids[i]);
	}

	int cont = N_CONSUMERS-1;
	for (i = 0; i < N_PRODUCERS; i++){
		array_threads_ids[cont] = cont;
		pthread_create(&producers[i],NULL,producer,(void*)&array_threads_ids[cont]);
		cont++;
	}

	for (i = 0; i < size_threads; i++){
		printf("%d ",array_threads_ids[i]);
	}

	pthread_exit(NULL); 
}