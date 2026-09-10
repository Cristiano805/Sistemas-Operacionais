#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <time.h>

#define TAM_VETOR 10000
#define NUM_FILHOS 8

typedef struct {
    int vetor[TAM_VETOR];
    double tempos_filhos[NUM_FILHOS];
} DadosCompartilhados;

bool verificaPos(int *vet, int tamanho) {
    if (tamanho <= 1) return true;
    int primeiro_valor = vet[0];
    for (int i = 1; i < tamanho; i++) {
        if (vet[i] != primeiro_valor) return false;
    }
    return true;
}

int main()
{
    DadosCompartilhados *dados = mmap(NULL, sizeof(DadosCompartilhados), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (dados == MAP_FAILED) {
        perror("Erro ao alocar memória compartilhada");
        return 1;
    }

    for (int i = 0; i < TAM_VETOR; i++) {
        dados->vetor[i] = 4;
    }

    int fatia = TAM_VETOR / NUM_FILHOS;

    for (int i = 0; i < NUM_FILHOS; i++) {
        pid_t pid_filho = fork();

        if (pid_filho < 0) {
            perror("Erro ao criar processo");
            exit(1);
        }

        if (pid_filho == 0)
        {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);

            int inicio = i * fatia;
            int fim = (i == NUM_FILHOS - 1) ? TAM_VETOR : (i + 1) * fatia;

            for (int j = inicio; j < fim; j++)
            {
                dados->vetor[j] = (dados->vetor[j] * 2) + 2;
            }

            clock_gettime(CLOCK_MONOTONIC, &end);
            dados->tempos_filhos[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
            exit(0); 
        }
    }

    for (int i = 0; i < NUM_FILHOS; i++) {
        wait(NULL);
    }

    bool iguais = verificaPos(dados->vetor, TAM_VETOR);
    if (iguais) {
        printf("Verificacao automatica: Todas as posicoes do vetor sao iguais a %d.\n", dados->vetor[0]);
    } else {
        printf("Verificacao automatica: As posicoes possuem valores diferentes.\n");
    }

    printf("\nTempos de execucao individuais (excluindo fork/wait):\n");
    for (int i = 0; i < NUM_FILHOS; i++) {
        printf("Filho %d: %.9f segundos\n", i, dados->tempos_filhos[i]);
    }

    munmap(dados, sizeof(DadosCompartilhados));
    return 0;
}