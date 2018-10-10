//
// Created by yuri on 10/10/18.
//

#ifndef T1_SISOP_FILTER_H
#define T1_SISOP_FILTER_H

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

void unlock (Filter *filter, int id){
    Filter *local = (Filter *) filter;
    local->level[id]=0;
}

void lock(Filter *filter, int id){

    Filter *local = (Filter *) filter;

    int i = 0;
    for (i = 1; i < local->tamanhoVetor; i++){
        local->level[id] = i;
        local->victim[i] = id;

        int j = 0;
        for (j = 0; j < local->tamanhoVetor; j++){
            while (j != id && local->level[j] >= i && local->victim[i] == id){}
        }

    }

}

#endif //T1_SISOP_FILTER_H
