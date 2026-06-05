#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int main(){
    // 1.创建套接字
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1){
        perror("socket");
        exit(-1);
    }
    //2.绑定地址端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd,(struct sockaddr*)&addr,sizeof(addr))==-1){
        perror("bind");
        exit(-1);
    }
    //3.监听
    listen(server_fd, 5);
    printf("等待客户端链接...\n");

    // 4.接收连接
    int client_fd = accept(server_fd, NULL, NULL);
    printf("客户端已连接...\n");

    // 5.循环收发数据
    char buf[1024];
    while(1){
        int n = read(client_fd, buf, sizeof(buf));
        if(n<=0)
            break;
        buf[n] = 0;
        printf("客户端：%s\n",buf);

        //回复消息
        char reply[1024]; 
        sprintf(reply, "服务端已收到：%s", buf);
        write(client_fd,reply,strlen(reply));
    }
    printf("客户端端口链接\n");
    close(client_fd);
    close(server_fd);
    return 0;
}