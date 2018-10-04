#include <stdio.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>
#include <stdlib.h>

#define N_PRODUCERS	3
#define N_CONSUMERS	1
#define N_BUFFER 6
sem_t mutex, empty, full;
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


void *producer(){
	srand(time(NULL)); 
	while(1){
		int item = rand()  % (65 + 1 - 0) + 0;
		printf("Produzindo o item %d\n",item);
		sem_wait(&empty);
		sem_wait(&mutex);
		enter_item(item);
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(){
	while (1){
		sem_wait(&full);
		sem_wait(&mutex);
		int data = remove_item();
		sem_post(&mutex);
		sem_post(&empty);
		printf("Consumindo item %d\n", data);
	}
}

void main (){
	sem_init(&mutex,0,1);
	sem_init(&empty,0,N_BUFFER);
	sem_init(&full,0,0);

	pthread_t producers[N_PRODUCERS], consumers[N_CONSUMERS];
	int i;
	for (i = 0; i < N_CONSUMERS; i++){
		pthread_create(&consumers[i],NULL,consumer,NULL);
	}

	for (i = 0; i < N_PRODUCERS; i++){
		pthread_create(&producers[i],NULL,producer,NULL);
	}
	pthread_exit(NULL); 
}