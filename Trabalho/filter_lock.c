#include "filter_lock.h"
#include <stdlib.h>
#include <stdio.h>

void init_filter_lock(Filter *filter,int size){


	filter->level = malloc( sizeof(int) * size);
	filter->victim = malloc( sizeof(int) * size-1);
	filter->array_size = size;

	int i;
	for (i = 0; i < size; i++){
		filter->level[i] = 0;
	}
    for (i = 0; i < size-1; i++){
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
        int sameOrHigher = 1;
        while (sameOrHigher && filter->victim[i] == id) {
            sameOrHigher = 0;
            for (j = 0; j < filter->array_size; j++) {
                if (j != id && filter->level[j] >= i) {
                    sameOrHigher = 1;
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