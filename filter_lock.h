//
// Created by yuri on 10/10/18.
//

#ifndef FILTER_LOCK_H
#define FILTER_LOCK_H

typedef struct{
    int *level;
    int *victim;
    int array_size;
}Filter;

void init_filter_lock(Filter *filter,int size);

void filter_unlock (Filter *filter, int id);

void filter_lock (Filter *filter, int id);

#endif //FILTER_LOCK_H
