#include "filter_lock.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


sem_t mutex_thread;

void init_filter_lock(Filter *filter,int size){


	filter->level = malloc( sizeof(int) * size);
	filter->victim = malloc( sizeof(int) * size);
	filter->array_size = size;

	int i;
	for (i = 0; i < size; i ++){
		filter->level[i] = 0;
		filter->victim[i] = 0;
	}


}

void filter_unlock (Filter *filter, int id){
    filter->level[id]=0;
}

void filter_lock(Filter *filter, int id){
    int i;
    for (i = 1; i < filter->array_size; i++){
        filter->level[id] = i;
        filter->victim[i] = id;

        int j;
        //for (j = 0; j < filter->array_size; j++){
        //    while (j != id && filter->level[j] >= i && filter->victim[i] == id){continue;}
        //}

        int someoneBigger = 1;
        while (filter->victim[i] == id && someoneBigger) {
            someoneBigger = 0;
            for (j = 0; j < filter->array_size; j++) {
                if(j == id) continue;
                if (filter->level[j] >= i) {
                    someoneBigger = 1;
                    break;
                }
            }
        }

    }

}

void destroy_filter_lock(Filter *filter){
    free(filter->level);
    free(filter->victim);

}