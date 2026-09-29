#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/sem.h>
#include <pthread.h>

#define TAM 10000
#define NUM_THREADS 100

int vetor[TAM];

void inicializaVetor(){
    int i = 0;
    while(i < TAM){
        vetor[i] = 10;
        i++;
    }
}

void* rotina_da_tarefa(void *arg){
    int id_tarefa = *(int *)arg;
    
    for (int i = 0; i < TAM; i++) {
      if(id_tarefa % 2 == 0){
        vetor[i] = vetor[i] + id_tarefa;
      } 
      else{
        vetor[i] = vetor[i] - id_tarefa;
      } 
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    int ids_tarefas[NUM_THREADS];
    inicializaVetor();

    for(int i = 0; i < NUM_THREADS; i++) {
        ids_tarefas[i] = i + 1;
        pthread_create(&threads[i], NULL, rotina_da_tarefa, &ids_tarefas[i]);
    }

} 