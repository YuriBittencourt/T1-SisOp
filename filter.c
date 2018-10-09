#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NUM_THREADS 2

int sharedTest = 1;

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
    for (i = 0; i < local->tamanhoVetor; i++){
    	local->level[id] = i;
    	local->victim[i] = id;

    	int k = 0;
    	while (k != id && (local->level[k] >= i && local->victim[i] == id)){k++;};
    }
}


void *counter(void * parameters){

	Parameters *local = (Parameters *) parameters;

	int i = 0;
	for (i = 0;  i < 1000; i ++){
		lock(local->filter, local->id);
		int temp = sharedTest;
		sharedTest = temp + 1;
		unlock(local->filter, local->id);
		printf("%d\n",sharedTest);
	}
	
}

void main (){

	int i = 0;
	int level[2], victim[2];

	for (i = 0; i < 2; i ++){
		level[i] = 0;
		victim[i] = 0;
	}

	Filter filter;
	filter.level = (int*) &level;
	filter.victim =(int*) &victim;

	pthread_t threads[NUM_THREADS];
    int t = 0;

	 for(t=0; t<NUM_THREADS; t++){
	 	Parameters parameter;
	 	parameter.filter = &filter;
	 	parameter.id = t;
       pthread_create(&threads[t], NULL, counter, (void *)&parameter);
    }
    pthread_exit(NULL);
}


