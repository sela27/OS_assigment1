#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int child_pid[5];
int child_index;

int signal_catcher()
{
    signal(SIGINT, signal_catcher);
    printf("PID %d caught one\n", getpid());
    if( child_index >= 0)
    {
        kill(child_pid[child_index] , SIGINT);
    }

}

int main()
{
    int stat;
    int pid;
    signal(SIGINT , signal_catcher);

    for(int i = 0; i < 5; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            printf("PID %d ready\n", getpid());
            child_index = i - 1;
            pause();
            exit(0);
        }
        else
        {
            child_pid[i] = pid;
        }
    }
    sleep(3);
    kill(child_pid[4] , SIGINT);
    sleep(3);
    for(int i = 0; i < 5; i++)
    {
        printf("Process %d is dead\n" , wait(&stat));
    }

    exit(0);

}