// epoll_server.c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

#define PORT 9999
#define MAX_EVENTS 1024

int main() {
    // 1. 创建监听socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr*)&addr, sizeof(addr));

    // 3. listen
    listen(lfd, 128);

    // 4. 创建epoll
    int epfd = epoll_create(MAX_EVENTS);

    // 5. 添加监听fd到epoll
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    printf("epoll 服务器已启动，端口：%d\n", PORT);

    while (1) {
        // 等待事件
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nready; i++) {
            int fd = events[i].data.fd;

            // 新连接
            if (fd == lfd) {
                int cfd = accept(lfd, NULL, NULL);
                printf("新客户端：%d\n", cfd);

                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            // 客户端消息
            else {
                char buf[1024];
                int n = read(fd, buf, sizeof(buf));

                if (n <= 0) {
                    close(fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    printf("客户端断开：%d\n", fd);
                } else {
                    buf[n] = 0;
                    printf("客户端%d：%s\n", fd, buf);
                    write(fd, buf, n);
                }
            }
        }
    }

    close(lfd);
    close(epfd);
    return 0;
}