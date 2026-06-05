#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "./my_fifo"

int main() {
    // 打开读端
    int fd = open(FIFO_PATH, O_RDONLY);

    char buf[128] = {0};
    read(fd, buf, sizeof(buf));
    printf("[读进程] 收到数据：%s\n", buf);

    close(fd);
    unlink(FIFO_PATH); // 删除管道文件
    return 0;
}