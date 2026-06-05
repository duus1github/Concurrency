// server_fork.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// 回收子进程，防止僵尸进程
void sig_child(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    // 处理子进程退出信号
    signal(SIGCHLD, sig_child);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    listen(server_fd, 5);
    printf("并发服务已启动，等待客户端连接...\n");

    while (1) {
        // 父进程循环接受连接
        int client_fd = accept(server_fd, NULL, NULL);
        printf("新客户端连接：fd=%d\n", client_fd);

        // 创建子进程
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程：负责通信
            close(server_fd); // 子进程不需要监听fd

            char buf[1024];
            while (1) {
                int n = read(client_fd, buf, sizeof(buf)-1);
                if (n <= 0) break;

                buf[n] = 0;
                printf("子进程%d 收到：%s\n", getpid(), buf);

                char reply[1024];
                sprintf(reply, "子进程%d 收到：%s", getpid(), buf);
                write(client_fd, reply, strlen(reply));
            }

            printf("子进程%d 客户端断开\n", getpid());
            close(client_fd);
            exit(0);
        } else {
            // 父进程：关闭客户端fd，继续accept
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}