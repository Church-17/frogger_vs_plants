#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <ncurses.h>

#define N_PROC 10

int main(){
    pid_t pid[N_PROC];

    for (int i = 0; i < N_PROC; i++)
    {
        pid[i] = fork();

        if (pid[i] > 0)
        {
            printf("[%d] Sono il processo padre di %d\n", i, pid[i]);
        }else if (pid[i] == 0)
        {
            printf("[%d] Sono il processo %d\n", i, (int)getpid());
        }else{perror("Errore");}

    }
}