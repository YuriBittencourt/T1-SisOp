#include <stdio.h>

typedef struct peterson{
    int *flag;
    int turn = 0;
}

void unlock(struct *peterson, int i){
    peterson->&flag[i]=0;
}

void lock(struct *peterson, int i){
    peterson->&flag[i]=1;
    turn = i;
    int j = i-1;
    if(j<0){
        j = len(peterson->&flag)-1
    }
    while(peterson->&flag[j] && turn==i){
        continue;
    }
}