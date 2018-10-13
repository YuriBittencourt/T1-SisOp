#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "filter_lock.h"
#define N 6


int postit = 0;

sem_t cheia, enchendo, create_thread;

Filter mutex;

void *usuario (void *arg){

	int id = *(int*)arg;
	while(1){
		sem_wait(&enchendo);
		filter_lock(&mutex,id);
		printf("\n usuário %d cola post it %d", id,postit);
		postit++;
		if (postit == N){
			sem_post(&cheia);
		}
		filter_unlock(&mutex,id);
	}
}

void *pombo(void *arg){
	int id = *(int*)arg;
	int j;
	while(1){
		sem_wait(&cheia);
		filter_lock(&mutex,id);
		printf("\npombo levando a mochila e voltando...");
		postit = 0;
		for(j = 0; j < N; j++)
			sem_post(&enchendo);
		filter_unlock(&mutex,id);
	}
}

void main(int argc, char *argv[]){
	int size_threads = N+1;

	pthread_t tpombo, tusuario[N];

	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));

	int i;
	

	init_filter_lock(&mutex,size_threads);

    sem_init(&cheia, 0, 0);
	sem_init(&enchendo, 0, N);
	sem_init(&create_thread,0,0);
 	
 	
    for (i = 0; i < size_threads-1; i ++){
    		array_threads_ids[i] = i;
    		pthread_create(&tusuario[i],NULL,usuario,(void*)&array_threads_ids[i]);
    }

    array_threads_ids[size_threads-1] = size_threads-1;
  	pthread_create(&tpombo, NULL, pombo, (void*)&array_threads_ids[size_threads-1]);

    pthread_exit(NULL);
    
  	destroy_filter_lock(&mutex);
}


