// 6.进程同步
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid<0){
        perror("fork");
        return -1;
    }
    if(pid==0){
        //子进程,睡眠2秒模拟干活
        printf("子进程 开始工作...\n");
        sleep(2);
        printf("子进程 结束工作...\n");
        return 123;
    }else{
        int status;
        printf("父进程 等待子进程退出...\n");
        pid_t ret = wait(&status);
        printf("收到子进程 pid=%d 退出\n", ret);
        printf("子退出码,%d\n", WEXITSTATUS(status));
    }
    return 0;
}