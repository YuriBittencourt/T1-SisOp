// Vinicius Duzac Cerutti (15280095-9)
// Yuri Ferreira Bittencourt (16105132-1)
// -----------------------------------------

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "filter_lock.h"

#define N_READERS	3
#define N_WRITERS	1

typedef struct{
	int *thread_id;
	int *id;
}Parameters;

sem_t w_db, r_db;

Filter mutex_rc, mutex_wc, mutex;
// mutex_rc e mutex_wc protegem a variaveis
// contadoras rc e wc respectivamente
// mutex - impede que + do que 1 leitor tente entrar na regiao critica
int rc = 0, wc = 0, reads = 0, writes = 0;

void *reader(void *arg){
	Parameters *pm = (Parameters*) arg;

	int thread_id = *(pm->thread_id);
	int id = *(pm->id);

	while(1){
		filter_lock(&mutex,thread_id);
		sem_wait(&r_db);
		filter_lock(&mutex_rc,thread_id);
		rc++;

		if (rc == 1) sem_wait(&w_db); // bloquia o acesso ao escritores

		filter_unlock(&mutex_rc,thread_id);
		sem_post(&r_db);
		filter_unlock(&mutex,thread_id);
		reads++;
		printf("(R) thread %d reading the database... (%d readers, %d reads, %d writes)\n", id, rc, reads, writes);
		filter_lock(&mutex_rc,thread_id);
		rc--;

		if (rc == 0) sem_post(&w_db); // indica a um escritor que este pode ter
									  // ter acesso aos dados

		filter_unlock(&mutex_rc,thread_id);
		printf("(R) thread %d using data...\n", id);
	}

}

void *writer(void *arg){
	Parameters *pm = (Parameters*) arg;

	int thread_id = *(pm->thread_id);
	int id = *(pm->id);

	while(1){
		filter_lock(&mutex_wc,thread_id);
		wc++;

		if (wc == 1) sem_wait(&r_db); //bloqueia o acesso ao escritores

		filter_unlock(&mutex_wc,thread_id);
		printf("(W) thread %d preparing data...\n", id);
		sem_wait(&w_db);
		writes++;
		printf("(W) thread %d writing to the database... (%d reads, %d writes)\n", id, reads, writes);
		sem_post(&w_db);
		filter_lock(&mutex_wc,thread_id);
		wc--;

		if (wc == 0)sem_post(&r_db); // permite que um processo leitor tente entrar
									 // na sua regiao critica
		
		filter_unlock(&mutex_wc,thread_id);
	}

}

int main(void){

	int size_threads = N_READERS + N_WRITERS; 
	pthread_t readers[N_READERS], writers[N_WRITERS];

	int *array_threads_ids = (int*)malloc(size_threads * sizeof(int));
	init_filter_lock(&mutex_rc,size_threads);
	init_filter_lock(&mutex_wc,size_threads);
	init_filter_lock(&mutex,size_threads);
	sem_init(&r_db,0, 1);
	sem_init(&w_db, 0, 1); // utilizado este semafaro, pois como um  ele pode ser usado
						   // por muitos leitores subsequentes sem que eles sejam bloqueados 
						   // novamente. Logo, com mutex não seria possivel já que a ideia eh
						   // esperar o recurso e liberar ao mesmo tempo (pela mesma thread).

	int i;

	int *ids_readers = (int*)malloc(N_READERS * sizeof(int));
	Parameters readers_parameters[N_READERS];


	// criando thread para leitores

	for(i = 0; i < N_READERS; i++){

		array_threads_ids[i] = i;
		ids_readers[i] = i;

		readers_parameters[i].thread_id = &array_threads_ids[i];
		readers_parameters[i].id = &ids_readers[i];

		pthread_create(&readers[i], NULL, reader, (void *)&readers_parameters[i]);
	}

	// para inicializar o vetor de ids para 
	// os escritores a partir da posicao N_READERS-1

	int cont = N_READERS-1;


	int *ids_writers = (int*)malloc(N_WRITERS * sizeof(int));
	Parameters writers_parameters[N_READERS];

	// criando thread para escritores
	for(i = 0; i < N_WRITERS; i++){
		array_threads_ids[cont] = cont;
		ids_writers[i] = i;

		writers_parameters[i].thread_id = &array_threads_ids[cont];
		writers_parameters[i].id = &ids_writers[i];
		
		pthread_create(&writers[i], NULL, writer, (void *)&writers_parameters[i]);
		cont++;
	}

	pthread_exit(NULL);	
	return(0);
}

