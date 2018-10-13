#include <pthread.h>
#ifndef FILTER_LOCK_H
#define FILTER_LOCK_H

typedef struct{
    int *level;
    int *victim;
    int array_size;
    int insert_at;
    pthread_t *threads_id;
}Filter;

void printID(pthread_t pt);

void init_filter_lock(Filter *filter,int size);

void filter_unlock (Filter *filter, int id);

void filter_lock (Filter *filter, int id);

void destroy_filter_lock(Filter *filter);
#endif //FILTER_LOCK_H
