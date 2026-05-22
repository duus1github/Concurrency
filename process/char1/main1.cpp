// 1.进程的创里
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    printf("当前进程pid:%d\n", getpid());
    pid_t pid = fork();

    if(pid>0){
        printf("父pid=%d,创建的子进程pid=%d \n", getpid(), pid);
    }else if(pid==0){
        printf("子pid=%d,父进程pid= %d\n",getpid(),getppid());
    }else{
        perror("fork failed");
        return -1;
    }
    printf("公共执行,进程pid = %d 执行完毕\n", getpid());
    return 0;
}
