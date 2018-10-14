#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "filter_lock.h"

#define N_THREADS 4

Filter mutex;

int share_memory = 0;
void * secaoCritica(void *arg){
	int id = *(int*)arg;
	srand(time(NULL));

	while(1){
		filter_lock(&mutex,id);
		printf("\nSeção critica Acessada com %d\n",id);
		printf("antigo share_memory %d\n",share_memory);
		share_memory = rand()  % (65 + 1 - 0) + 0;
		printf("gerei %d\n",share_memory);
		filter_unlock(&mutex,id);
	}
}

void main(int argc, char *argv[]){

	int size_threads = N_THREADS;

	pthread_t tusuario[N_THREADS];

	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));

	init_filter_lock(&mutex,size_threads);

	int i;
 	
 	printf("\nInicializando criacao de threads: \n");

    for (i = 0; i < size_threads; i ++){
    		array_threads_ids[i] = i;
    		pthread_create(&tusuario[i],NULL,secaoCritica,(void*)&array_threads_ids[i]);
    }

    pthread_exit(NULL);
    
  	destroy_filter_lock(&mutex);
}