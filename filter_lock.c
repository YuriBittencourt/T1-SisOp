#include "filter_lock.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


sem_t mutex_thread;

void printID(pthread_t pt){
    unsigned char *ptc = (unsigned char*)(void*)(&pt);
    for (size_t i=0; i<sizeof(pt); i++) {
        printf("%02x", (unsigned)(ptc[i]));
    }
}

void init_filter_lock(Filter *filter,int size){

    sem_init(&mutex_thread, 0, 1);

	filter->level = malloc( sizeof(int) * size);
	filter->victim = malloc( sizeof(int) * size);
	filter->array_size = size;

    filter->insert_at = 0;
    filter->threads_id = malloc( sizeof(pthread_t) * size);

	int i;
	for (i = 0; i < size; i ++){
		filter->level[i] = 0;
		filter->victim[i] = 0;
	}


}

void filter_unlock (Filter *filter, int id){
    //int id = (int) pthread_self();
    //printf("%d\n", id);
    filter->level[id]=0;
}

void filter_lock(Filter *filter, int id){
    //int id = (int) pthread_self();
    //printf("%d\n", id);
    int i;
    for (i = 1; i < filter->array_size; i++){
        filter->level[id] = i;
        filter->victim[i] = id;

        int j;
        for (j = 0; j < filter->array_size; j++){
            while (j != id && filter->level[j] >= i && filter->victim[i] == id){continue;}
        }

    }

}

/*void filter_createIndexThread(Filter *filter,pthread_t thread){
    sem_wait(&mutex_thread);

    if(filter_findIndex(filter,thread) != -1){
        sem_post(&mutex_thread);
        return;
    }

    if (filter->insert_at != filter->array_size){
        int index = filter -> insert_at;
        filter->threads_id[index] = thread;
        filter -> insert_at = filter -> insert_at + 1;
    }
    sem_post(&mutex_thread);
}

int filter_findIndex(Filter *filter,pthread_t thread){

    int i;
   
    for (i = 0; i < filter -> array_size; i++){
        if (pthread_equal(thread,filter -> threads_id[i])){
            return i;
        }
    }
    return -1;
}
*/