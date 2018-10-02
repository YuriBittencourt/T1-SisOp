#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define N_PRODUCERS	3
#define N_CONSUMERS	1
#define N_BUFFER 6
sem_t mutex, empty, full;
int buffer[N_BUFFER];
int insertCountBuffer = -1;
int removeCountBuffer = 0;
int itemCountBuffer = 0;

void enter_item (int data){
    if (itemCountBuffer != N_BUFFER){
         if (insertCountBuffer == N_BUFFER -1){
             insertCountBuffer =-1;
         }
         buffer[++insertCountBuffer] = data;
         itemCountBuffer++;
    }
}

int remove_item (){
    int data = buffer[removeCountBuffer++];
    if (removeCountBuffer == N_BUFFER){
        removeCountBuffer = 0;
    }
    itemCountBuffer--;
    return data;
}

void *producer(void *arg){
	long int i;

	i = (long int)arg;
    while(1){
        srand(time(NULL));  
        int item = rand();
        printf("\nProduzindo item %d",item);
        sem_wait(&empty);
        sem_wait(&mutex);
        enter_item(item);
        sem_post(&mutex);
        sem_post(&full);
    }
        
}

void *consumer(void *arg){
	long int i;

	i = (long int)arg;
    while(1){
        sem_wait(&full);
        sem_wait(&mutex);
        int item = remove_item();
        sem_post(&mutex);
        sem_post(&empty);
        printf("\nConsumido item %d",item);
    }
}


int main(void){
	long int i;
	pthread_t producers[N_PRODUCERS], consumers[N_CONSUMERS];

	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N_BUFFER);	
	sem_init(&full, 0, 0);

	for(i = 0; i < N_PRODUCERS; i++)
		pthread_create(&producers[i], NULL, producer, (void *)i);

	for(i = 0; i < N_CONSUMERS; i++)
		pthread_create(&consumers[i], NULL, consumer, (void *)i);

	pthread_exit(NULL);	
	return(0);
}