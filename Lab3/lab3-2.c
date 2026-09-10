#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define TAM_VETOR 10000
#define NUM_THREADS 8 

int vetor[TAM_VETOR];
double tempo_threads[NUM_THREADS];

bool verificaPos(int *vet, int tamanho) {
    if (tamanho <= 1) return true;
    int primeiro_valor = vet[0];
    for (int i = 1; i < tamanho; i++) {
        if (vet[i] != primeiro_valor) return false;
    }
    return true;
}

void* trabalhador(void* arg) {
    int id = *(int*)arg; // id da thread
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int j = 0; j < TAM_VETOR; j++) {
        vetor[j] = (vetor[j] * 2) + 2;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    tempo_threads[id] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
                         
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < TAM_VETOR; i++) {
        vetor[i] = 4;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, trabalhador, &ids[i]) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    bool iguais = verificaPos(vetor, TAM_VETOR);
    if (iguais) {
        printf("Verificacao: Sucesso! Todas as posicoes sao iguais a %d.\n", vetor[0]);
    } else {
        printf("Verificacao: Falha! Ocorreu inconsistencia nos dados (Condicao de Corrida).\n");
    }

    printf("\nTempos de execucao puros (excluindo criacao/termino):\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d: %.9f segundos\n", i, tempo_threads[i]);
    }

    return 0;
}