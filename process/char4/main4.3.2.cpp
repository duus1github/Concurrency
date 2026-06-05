
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid == 0){
        close(fd[1]);
        char buf[32] = {0};
        int n = read(fd[0], buf, sizeof(buf));
        printf("[子进程] read 返回值：%d(代表管道已关闭) \n", n);
        close(fd[0]);
        return 0;
    }else{
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}