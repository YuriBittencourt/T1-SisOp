#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "filter_lock.h"

#define N_READERS	3
#define N_WRITERS	1

sem_t resource;
Filter mutex_rc, mutex_wc, read_try;

int rc = 0, wc = 0, reads = 0, writes = 0;

void *reader(void *arg){
	int id = *(int*)arg;
	while(1){
		filter_lock(&read_try,id);
		filter_lock(&mutex_rc,id);
		reads++;
		rc++;
		if(rc == 1){sem_wait(&resource);}
		filter_unlock(&mutex_rc,id);
		filter_unlock(&read_try,id);
		printf("(R) thread %d reading the database... (%d readers, %d reads, %d writes)\n", id, rc, reads, writes);
		filter_lock(&mutex_rc,id);
		rc--;
		if (rc == 0) {sem_post(&resource);}
		filter_unlock(&mutex_rc,id);
		printf("(R) thread %d using data...\n", id);
	}
}

void *writer(void *arg){
	int id = *(int*)arg;
	while(1){
		filter_lock(&mutex_wc,id);
		wc++;
		if (wc == 1){filter_lock(&read_try,id);}
		filter_unlock(&mutex_wc,id);
		printf("(W) thread %d preparing data...\n", id);
		sem_wait(&resource);
		writes++;
		printf("(W) thread %d writing to the database... (%d reads, %d writes)\n", id, reads, writes);
		sem_post(&resource);
		filter_unlock(&mutex_wc,id);
		wc--;
		if (wc == 0) {filter_unlock(&read_try,id);}
		filter_unlock(&mutex_wc,id);
	}
}

int main(void){
	long int i;
	int size_threads = N_READERS + N_WRITERS; 
	pthread_t readers[N_READERS], writers[N_WRITERS];

	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));

	init_filter_lock(&mutex_rc,size_threads);
	init_filter_lock(&mutex_wc,size_threads);
	init_filter_lock(&read_try,size_threads);
	sem_init(&resource, 0, 1);

	for(i = 0; i < N_READERS; i++){
		array_threads_ids[i] = i;
		pthread_create(&readers[i], NULL, reader, (void *)&array_threads_ids[i]);
	}

	int cont = N_READERS-1;
	for(i = 0; i < N_WRITERS; i++){
		array_threads_ids[cont] = cont;
		pthread_create(&writers[i], NULL, writer, (void *)&array_threads_ids[cont]);
		cont++;
	}

	pthread_exit(NULL);	
	return(0);
}

