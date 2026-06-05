// flock排他锁多进程争抢文件
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/wait.h>

#define FILE_PAHT "./test_lock.txt"

void work(int idx){
    int fd = open(FILE_PAHT, O_RDONLY | O_CREAT, 0664);
    if(fd<0){
        perror("open");
        return;
    }
    printf("进程%d:尝试获取排他锁\n", idx);
    //加阻塞排他锁
    if(flock(fd,LOCK_EX)<0){
        perror("flock");
        close(fd);
        return;
    }
    printf("进程%d:拿到锁，开始操作文件（3s）\n",idx);
    sleep(3);
    flock(fd, LOCK_UN);
    printf("进程%d:释放锁\n", idx);
    close(fd);
}

int main(){
    for (int i = 1; i <= 3;i++){
        pid_t pid = fork();
        if(pid==0){
            work(i);
            _exit(0);
        }
    }
    //等待全部子进程结束
    for (int i = 1; i <= 3;i++)
        wait(NULL);
    return 0;
}