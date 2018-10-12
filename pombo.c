#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "filter_lock.h"
#define N 6


int postit = 0;

sem_t cheia, enchendo;

Filter mutex;

void printArray(int array[], int size){
	int i;
	printf("\n");
	for (i = 0; i < size; i ++){
		printf ("%d ",array[i]);
	}
	printf("\n");
}

void *usuario (void *arg){
	int i;

	i = (long int) arg;

	while(1){
		sem_wait(&enchendo);
		//sem_wait(&mutex);
		filter_lock(&mutex, i);
		printf("\n usuário %d cola post it %d", i,postit);
		postit++;
		if (postit == N){
			sem_post(&cheia);
		}
		//sem_post(&mutex);
		filter_unlock(&mutex, i);
	}
}

void *pombo(void *arg){
	int i;
	i = (long int) arg;

	while(1){
		sem_wait(&cheia);
		//sem_wait(&mutex);
		filter_lock(&mutex,i);
		printf("\npombo levando a mochila e voltando...");
		postit = 0;
		for(i = 0; i < N; i++)
			sem_post(&enchendo);
		filter_unlock(&mutex,i);
		//sem_post(&mutex);
	}
}

void main(int argc, char *argv[]){
	int size_threads = N+1;
	int indexes[size_threads];

	init_filter_lock(&mutex,size_threads);

	pthread_t tpombo, tusuario[N];

    sem_init(&cheia, 0, 0);
	sem_init(&enchendo, 0, N);
	
 	
 
 	int i;
    for (i = 0; i < size_threads; i ++){
    	printf("%d",i);
    	indexes[i] = i;
    	if (i == 0 ){
    		pthread_create(&tpombo, NULL, pombo, (void*)&indexes[i]);
    	}else{
    		pthread_create(&tusuario[i],NULL,usuario,(void*)&indexes[i]);
    	}
    }
    
    pthread_exit(NULL);
}


