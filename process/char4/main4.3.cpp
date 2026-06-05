// 功能：父进程写入数据，子进程读取数据并打印
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    int fd[2];
    int ret = pipe(fd);
    if (ret ==-1){
        perror("pipe create error");
        return -1;
    }
    //2.创建子进程
    pid_t pid = fork();
    if(pid == -1){
        perror("fork error");
        return -1;
    }
    //2.子进程读取数据
    if(pid == 0){
        close(fd[1]);//子进程子需要读，关闭写端
        char buf[128] = {0};
        read(fd[0], buf, sizeof(buf));
        printf("[子进程] 从管道读取到：%s ,%d\n", buf,pid);
        close(fd[0]);
        return 0;
    }else{ //3.父进程写数据
        //父进程只需要写，关闭读端
        printf("[父进程] pid:%d \n", pid);
        close(fd[0]);
        const char *msg = "hello pipe,这是父进程发送的消息";
        write(fd[1], msg, strlen(msg));
        printf("[父进程] 向管道发生数据完成\n");
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}