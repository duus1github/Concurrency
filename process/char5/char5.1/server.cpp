// server.c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // 1. 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定IP+端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);  // 端口8888
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    // 3. 监听
    listen(server_fd, 5);
    printf("等待客户端连接...\n");

    // 4. 接受客户端连接
    int client_fd = accept(server_fd, NULL, NULL);
    printf("客户端已连接\n");

    // 5. 收发数据（这里简单演示）
    char buf[1024] = "你好，我是服务端";
    write(client_fd, buf, sizeof(buf));

    // 6. 关闭
    close(client_fd);
    close(server_fd);
    return 0;
}