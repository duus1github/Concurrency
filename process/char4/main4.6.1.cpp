#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuf{
    long mtype;
    char mtext[128];
};

int main(){
    //生成key
    key_t key = ftok(".",66);
    //2.消息队列
    int msgid = msgget(key,0666|IPC_CREAT);

    pid_t pid = fork();
    if(pid ==0){
        struct msgbuf snd;
        snd.mtype = 1;
        strcpy(snd.mtext,"我是子进程，这是消息队列消息");
        msgsnd(msgid,&snd,strlen(snd.mtext),0);
        printf("[子进程] 消息发送成功\n");
        exit(0);
    }else{
        wait(NULL);
        struct msgbuf rcv;
        msgrcv(msgid,&rcv,sizeof(rcv.mtext),1,0);
        printf("[父进程] 收到消息：%s\n",rcv.mtext);
        //删除队列
        msgctl(msgid,IPC_RMID,NULL);
    }
    return 0 ;
}