#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(){
    // 创建socket
    int fd = socket(AF_INET,SOCK_STREAM,0);
    // 连接服务端
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //链接
    connect(fd, (struct sockaddr *)&addr, sizeof(addr));

    //3.读取数据
    char buf[1024];
    read(fd,buf,sizeof(buf));
    printf("收到：%s\n", buf);
    close(fd);
    return 0;
}