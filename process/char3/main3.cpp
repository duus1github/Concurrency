#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid>0){
        //父进程:等待子进程执行完毕
        printf("[父进程] pid=%d,等待子进程执行ls命令\n", getpid());
        wait(NULL); //等待子进程结束
        printf("[父进程] 子进程执行完毕\n");
    }else if(pid == 0){
        //子进程:执行ls命令
        printf("[子进程] pid = %d,开始执行ls -l \n",getpid());
        execlp("ls", "ls", "-a", NULL);
        perror("execlp error");
        return 1;
    }else{
        perror("fork failed");
        return -1;
    }
    return 0;
}