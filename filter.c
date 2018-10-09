#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define N	20

int postit = 0;

sem_t mutex, cheia, enchendo;

typedef struct{
    int *level;
    int *victim;
    int tamanhoVetor;
}Filter;

typedef struct 
{
	Filter *filter;
	int id;
}Parameters;



void printArray(int array[], int size){
	int i;
	printf("\n");
	for (i = 0; i < size; i ++){
		printf ("%d ",array[i]);
	}
	printf("\n");
}

void unlock (Filter *filter, int id){
    Filter *local = (Filter *) filter;
    local->level[id]=0;
}

void lock(Filter *filter, int id){
	
    Filter *local = (Filter *) filter;
    
    int i = 0;
    for (i = 0; i < local->tamanhoVetor; i++){
    	local->level[id] = i;
    	local->victim[i] = id;

    	printArray(local->level,local->tamanhoVetor);
 
    	int conflicts_exist = 1;
		while (conflicts_exist) {
			conflicts_exist = 0;
			int k = 0;
			for (k = 0; k < local->tamanhoVetor; k++) {

				if (k != id && local->level[k] >= i && local->victim[i] == id) {

					 conflicts_exist = 1;
					 break;
				}
			}
		}
	}
}

void *usuario (void *args){
	Parameters *local = (Parameters *) args;
	while(1){
		sem_wait(&enchendo);
		//sem_wait(&mutex);
		lock(local->filter, local->id);
		printf("\n usuário %d cola post it %d", local->id,postit);
		postit++;
		if (postit == N){
			sem_post(&cheia);
		}
		//sem_post(&mutex);
		unlock(local->filter, local->id);
	}
}

void *pombo(void *args){
	Parameters *local = (Parameters *) args;
	int i;

	while(1){
		sem_wait(&cheia);
		//sem_wait(&mutex);
		lock(local->filter,local->id);
		printf("\npombo levando a mochila e voltando...");
		postit = 0;
		for(i = 0; i < N; i++)
			sem_post(&enchendo);
		unlock(local->filter,local->id);
		//sem_post(&mutex);
	}
}

void main (){

	int size_threads = N+1;
	int i = 0;
	int level[size_threads], victim[size_threads];

	for (i = 0; i < size_threads; i ++){
		level[i] = 0;
		victim[i] = 0;
	}

	Filter filter;
	filter.level = (int*) &level;
	filter.victim =(int*) &victim;
	filter.tamanhoVetor = size_threads;

	pthread_t tpombo, tusuario[N];

    sem_init(&cheia, 0, 0);
	sem_init(&enchendo, 0, N);
	sem_init(&mutex, 0, 1);

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


