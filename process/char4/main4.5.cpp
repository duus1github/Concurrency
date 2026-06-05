#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        //子进程
        printf("子进程开始运行，3秒后退出\n");
        sleep(3);
        printf("子进程退出\n");
    }else{
        printf("父进程等待子进程...\n");
        wait(NULL);
        printf("父进程：子进程已退出，我继续运行\n");
    }
    return 0;

}