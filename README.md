#  [Sistemas Operacionais - Trabalho Prático I](https://github.com/YuriBittencourt/T1-SisOp/blob/master/enunciadoT1.pdf) #

O primeiro trabalho prático da disciplina de Sistemas Operacionais consiste na implementação de primitivas para garantir a exclusão mútua (mutex) inteiramente por software (ou seja, utilizando espera ocupada). As primitivas a serem implementadas são enter region() (ou lock()) e leave region() (ou unlock()).

As primitivas implementadas não devem utilizar semáforos nem mecanismos de exclusão mútua já presentes na biblioteca pthreads. Escolha algum algoritmo que implementa exclusão mútua por software e que seja generalizado para <i>N processos / threads</i> - por exemplo o algoritmo de Dekker ou o algoritmo de Peterson. 

## Objetivos: ##
* - [x] Criar primitivas para garantir mutex inteiramente por software (*lock(), unlock()*)
* - [x] Aplicar em:
  * - [x] Produtores/Consumidores
  * - [x] Leitores/Escritores com prioridade para os escritores
## Compilar os Arquivos: ##
```
gcc -o nome_do_arquivo nome_do_arquivo.c filter_lock.c filter_lock.h -lpthread
```
###  Números de Threads: ### 
Os números de Threads em cada programa foram definidos atráves de macros (\#define).
```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "filter_lock.h"

#define N_READERS	3
#define N_WRITERS	1
```
### Uso do Filter Lock: ###
Basta fazer include do arquivo filter_lock.h no programa principal e realizar a compilação. Para criar um "mutex" necessita criar um Filter e declar na main a inicialização do mutex:
```c
#include "filter_lock.h"
Filter mutex;

void main(){
init_filter_lock(&mutex,num_threads);
}
```
* `filter_lock()` equivale a P ou wait .
* `filter_unlock()` equivale a V ou post.
