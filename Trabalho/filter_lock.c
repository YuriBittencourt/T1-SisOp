// Vinicius Duzac Cerutti (15280095-9)
// Yuri Ferreira Bittencourt (16105132-1)
// -----------------------------------------


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
    // incrementa o level da thread conforme, o numero de threads
    // tentando acessar o recurso level-1 = acesso a SC e 
    // level 1 menor prioridade
    for (i = 1; i < filter->array_size; i++){
        filter->level[id] = i;
        filter->victim[i] = id;

        int j;
        // matenha esta thread no loop desde que nenhuma
        // outra thread está no mesmo ou maior nível e enquanto eu não
        // conseguir nova vítima.
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

void destroy_filter(Filter *filter){
    free(filter->level);
    free(filter->victim);

}