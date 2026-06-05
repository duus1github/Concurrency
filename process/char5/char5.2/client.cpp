#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    // 1.创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    // 2.链接服务端
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(fd,(struct sockaddr*)&addr,sizeof(addr))==-1){
        perror("connect");
        exit(1);
    }
    // 3.循环发送消息
    char buf[1024];
    while(1){
        printf("请输入消息：");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0; //去掉换行

        if(strcmp(buf,"quit")==0)
            break;
        write(fd,buf,strlen(buf));

        //读取回复
        int n = read(fd,buf,sizeof(buf)-1);
        if(n<=0)
            break;
        buf[n] = 0;
        printf("%s\n",buf);

    }
    close(fd);
    return 0;
}
