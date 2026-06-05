// 互斥锁保护全局变量

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int g_count = 0; //共享资源
pthread_mutex_t mutex;//互斥锁

void* func(void *arg){
    for (int i = 0; i < 100000;i++){
        pthread_mutex_lock(&mutex); //加锁
        g_count++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_t t1, t2;
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&t1,NULL,func,NULL);
    pthread_create(&t2, NULL, func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("g_count = %d\n",g_count);
    pthread_mutex_destroy(&mutex);
    return 0;
}
