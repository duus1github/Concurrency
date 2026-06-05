#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 128

int main(){
    key_t key = ftok(".",66);
    int shmid = shmget(key,SIZE,0666|IPC_CREAT);
    pid_t pid = fork();
    if(pid==0){
        //子进程：挂载+写数据
        char *p = (char*)shmat(shmid,NULL,0);
        strcpy(p,"我在共享内存里！");
        printf("[子进程] 写入共享内存完成\n");
        shmdt(p);
        _exit(0);
    }else{
        wait(NULL);
        //父进程：挂载+读数据
        char *p = (char*)shmat(shmid,NULL,0);
        printf("[父进程] 从共享内存读取数据：%s\n",p);
        shmdt(p);
        shmctl(shmid,IPC_RMID,NULL);
    }
    return 0;
}