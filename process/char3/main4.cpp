/**
 * 1.父进程打开一个文本文件
 * 2.fork子进程
 * 3.看变量各自独立,文件偏移共享
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
int g_val = 100;
int main()
{
    int local_val = 200;
    int fd = open("test.txt", O_RDWR | O_CREAT, 0664);
    if(fd < 0 ){
        perror("open failed");
        return -1;
    }
    printf("父进程:打开文件 fd=%d\n",getpid());
    //写入初始内容
    write(fd, "123456789", 9);
    //创建子进程
    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed");
        close(fd);
        return -1;
    }
    if (pid==0){
        printf("======子进程========\n");
        printf("[子进程] pid=%d 继承 fd=%d \n", getpid(), fd);
        //修改全局变量,局部变量:只会改自己的副本,不影响父进程
        g_val = 999;
        local_val = 888;
        printf("[子进程] g_val=%d,local_val=%d\n", g_val, local_val);
        write(fd, "abc", 3);
        close(fd);
    }else{
        printf("======父进程========\n");
        sleep(1); //等待子进程先执行
        
        printf("[父进程] pid=%d\n", getpid());
        printf("[父进程] g_val=%d,local_val=%d\n",g_val,local_val);

        //读取文件全部内容,验证偏移共享
        lseek(fd, 0, SEEK_SET);
        char buf[32] = {0};
        read(fd, buf, sizeof(buf) - 1);
        printf("[父进程] 读取文件内容: %s\n", buf);
        close(fd);
    }
    return 0;
}