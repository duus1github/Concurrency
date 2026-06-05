// 生产者-消费者模型
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int g_data = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

//消费者：等待数据
void * consumer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        while(g_data == 0)
            pthread_cond_wait(&cond,&mutex);

        printf("消费者:取走数据%d\n",g_data);
        g_data = 0;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

}
// 生产者：生产数据
void * producer(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        g_data = 100 + rand() % 10;
        printf("生产者生产数据：%d \n", g_data);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
}

int main(){
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, consumer, NULL);
    pthread_create(&t2,NULL,producer,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}