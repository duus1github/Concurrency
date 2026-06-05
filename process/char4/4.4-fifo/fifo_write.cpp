#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FIFO_PATH "./my_fifo"

int main() {
    // 创建有名管道
    mkfifo(FIFO_PATH, 0666);
    // 打开写端
    int fd = open(FIFO_PATH, O_WRONLY);

    const char* msg = "Hello FIFO! 我是写进程";
    write(fd, msg, strlen(msg));
    printf("[写进程] 数据发送成功\n");

    close(fd);
    return 0;
}