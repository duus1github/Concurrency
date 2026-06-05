// select_server.c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>

#define PORT 9999
#define MAX_FD 1024

int main() {
    // 1. 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr*)&addr, sizeof(addr));

    // 3. listen
    listen(lfd, 128);

    // 4. select 准备
    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(lfd, &allset);  // 监听套接字加入集合
    int maxfd = lfd;

    printf("select 服务器已启动，端口：%d\n", PORT);

    while (1) {
        rset = allset;
        // 监听读事件
        int nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        // 有新连接
        if (FD_ISSET(lfd, &rset)) {
            int cfd = accept(lfd, NULL, NULL);
            printf("新客户端连接：%d\n", cfd);

            FD_SET(cfd, &allset);
            if (cfd > maxfd) maxfd = cfd;

            if (--nready == 0) continue;
        }

        // 遍历已连接客户端
        for (int i = lfd + 1; i <= maxfd; i++) {
            if (FD_ISSET(i, &rset)) {
                char buf[1024];
                int n = read(i, buf, sizeof(buf));

                if (n <= 0) {
                    close(i);
                    FD_CLR(i, &allset);
                    printf("客户端断开：%d\n", i);
                } else {
                    buf[n] = 0;
                    printf("客户端%d：%s\n", i, buf);
                    write(i, buf, n);
                }

                if (--nready == 0) break;
            }
        }
    }

    close(lfd);
    return 0;
}