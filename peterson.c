#include <stdio.h>
#include <stdlib.h>

struct Peterson{
    int *flag;
    int turn;
    int tamanhoVetor;
};

void unlock (struct Peterson *peterson, int i){
    peterson->flag[i]=0;
}

void lock(struct Peterson *peterson, int i){
    peterson->flag[i]=1;
    peterson->turn = i;
    int j = (i + (peterson->tamanhoVetor)-1) % (peterson->tamanhoVetor);
    while(peterson->flag[j] && peterson->turn==i){
        continue;
    }
}

void main (){
    struct Peterson peterson1;

    peterson1.flag = (int*) malloc (sizeof (int) * 2);
    peterson1.tamanhoVetor = 4;
    peterson1.flag[1] = 10;

    unlock(&peterson1, 1);

    int j = (1 + (peterson1.tamanhoVetor)-1) % (peterson1.tamanhoVetor);
    printf("%d",j);
}