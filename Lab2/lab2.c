#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main()
{
    int *p, i, id;

    id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (id < 0) {
        perror("Deu erro na criacao de memoria compartilhada.");
        exit(-1);
    }

    p = (int *) shmat(id, NULL, 0);
    if (p == (int *) -1) { 
        perror("Deu erro na anexacao.");
        exit(-1);
    }
    
    *p = 1;
    
    pid_t pid_filho, pid_neto;
    pid_filho = fork();

    if (pid_filho == 0)
    {
        pid_neto = fork();

        if (pid_neto == 0)
        {
            for (i = 0; i < 1000; i++)
            {
                (*p) += 3;
                printf("processo neto, pid=%d, n=%d\n", getpid(), *p);
            }
            shmdt(p);
            exit(0);
        }

        for (i = 0; i < 1000; i++)
        {
            (*p) += 2;
            printf("processo filho, pid=%d, n=%d\n", getpid(), *p);
        }

        wait(NULL);
        shmdt(p);
        exit(0);
    }

    for (i = 0; i < 1000; i++)
    {
        (*p) += 1;
        printf("processo pai, pid=%d, n=%d\n", getpid(), *p);
    }
    printf("Valor final: %d", *p);
    wait(NULL); 

    shmdt(p);
    shmctl(id, IPC_RMID, 0);

    return 0;
}