#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "filter_lock.h"
#define N 6


typedef struct
{
    Filter *filter;
    int id;
}Parameters;


int postit = 0;

sem_t mutex, cheia, enchendo;

void printArray(int array[], int size){
	int i;
	printf("\n");
	for (i = 0; i < size; i ++){
		printf ("%d ",array[i]);
	}
	printf("\n");
}

void *usuario (void *args){
	Parameters *local = (Parameters *) args;
	while(1){
		sem_wait(&enchendo);
		//sem_wait(&mutex);
		filter_lock(local->filter, local->id);
		printf("\n usuário %d cola post it %d", local->id,postit);
		postit++;
		if (postit == N){
			sem_post(&cheia);
		}
		//sem_post(&mutex);
		filter_unlock(local->filter, local->id);
	}
}

void *pombo(void *args){
	Parameters *local = (Parameters *) args;
	int i;

	while(1){
		sem_wait(&cheia);
		//sem_wait(&mutex);
		filter_lock(local->filter,local->id);
		printf("\npombo levando a mochila e voltando...");
		postit = 0;
		for(i = 0; i < N; i++)
			sem_post(&enchendo);
		filter_unlock(local->filter,local->id);
		//sem_post(&mutex);
	}
}

void main(int argc, char *argv[]){
	int size_threads = N+1;
	int i = 0;
	int level[size_threads], victim[size_threads];

	for (i = 0; i < size_threads; i ++){
		level[i] = 0;
	}

	Filter filter;
	filter.level = (int*) &level;
	filter.victim =(int*) &victim;
	filter.array_size = size_threads;

	pthread_t tpombo, tusuario[N];

    sem_init(&cheia, 0, 0);
	sem_init(&enchendo, 0, N);

    Parameters parameter;
 	parameter.filter = &filter;
 	parameter.id = 0;

 	pthread_create(&tpombo, NULL, pombo, (void *)&parameter);

    for (i = 1; i <= N ; i ++){
    	Parameters parameter;
	 	parameter.filter = &filter;
	 	parameter.id = i;
    	pthread_create(&tusuario[i-1],NULL,usuario,(void*)&parameter);
    }
    
    pthread_exit(NULL);
}


