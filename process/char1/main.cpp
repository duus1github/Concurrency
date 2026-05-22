#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid>0){
        //父进程:等待子进程执行完毕
        printf("父进程pid=%d,等待子进程执行ls命令\n", getpid());
    }
}