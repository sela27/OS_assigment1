#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include "md5.h"

const char* s;
void check_md5(int signal)
{
    if(strlen(s) == 32)
    {
        exit(0);
    }
}

int main()
{
    int pid;
    signal(SIGINT , check_md5);
    int f[2];
    int f2[2];
    if(pipe(f) == -1 || pipe(f2) == -1)
    {
        printf("piping faild");
        exit(1);
    }

    pid = fork();
    if( pid > 0)
    {
        char buffer[20];
        printf("plain text: ");
        scanf("%s" , buffer);
        char enc[33];
        close(f[0]);
        write(f[1] , buffer, strlen(buffer) + 1);
        close(f[1]);
        wait(NULL);
        sleep(2);
        close(f2[1]);
        read(f2[0] , enc , 33);
        printf("encrypted by process %d : %s\n" ,pid, enc);
        kill(pid , SIGKILL);
        close(f2[0]);
    }
    else if(pid == 0)
    {
        close(f[1]);
        char buffer[20];
        read(f[0] , buffer , 20);
        s = md5(buffer).c_str();
        close(f[0]);
        close(f2[0]);
        write(f2[1] , s , strlen(s) + 1);
        close(f2[1]);
        kill(getgid(), SIGINT);
    }
    else
    {
        printf("forking faild");
        exit(1);

    }
    exit(0);
}