#include "filter_lock.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void init_filter_lock(Filter *filter,int size){


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
    //int id = filter_findIndex(filter,thread);
    filter->level[id]=0;
}

void filter_lock(Filter *filter, int id){
    //int id = filter_findIndex(filter,thread);
    int i;
    for (i = 1; i < filter->array_size; i++){
        filter->level[id] = i;
        filter->victim[i] = id;

        int j = 0;
        for (j = 0; j < filter->array_size; j++){
            while (j != id && filter->level[j] >= i && filter->victim[i] == id){}
        }

    }

}


/*int filter_findIndex(Filter *filter,pthread_t thread){

    int i;
    for (i = 0; i < filter -> array_size; i++){
        if (pthread_equal(thread,filter ->threads_id[i])){
            return i;
        }
    }

    if (filter->insert_at != filter->array_size){
        int index = filter->insert_at ;
        filter->threads_id[index] = thread;
        filter->insert_at = filter->insert_at  + 1;
        return index;

    }

    return -1;
}*/