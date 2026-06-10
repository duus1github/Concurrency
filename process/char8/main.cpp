// thread pool
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#define MAX_THREADS 5
#define MAX_QUEUE 100

typedef struct{
    void (*func)(void *); //任务函数
    void *arg;            //函数参数
} task_t;
typedef struct
{
    pthread_t threads[MAX_THREADS];
    task_t queue[MAX_QUEUE];

    int front;
    int rear;
    int count;

    pthread_mutex_t mutex; //互斥锁
    pthread_cond_t cond;   //条件变量
    int shutdown;          //关闭标记

} threadpool_t;

threadpool_t pool;

//线程工作函数
void * thread_work(void *arg){
    while(1){
        pthread_mutex_lock(&pool.mutex);
        //无任务，未关闭 -> 等待
        while(pool.count == 0 && !pool.shutdown)
            pthread_cond_wait(&pool.cond, &pool.mutex);
        
        //受到关闭信号
        if(pool.shutdown){
            pthread_mutex_unlock(&pool.mutex);
            pthread_exit(NULL);
        }
        //取任务
        task_t task = pool.queue[pool.front];
        pool.front = (pool.front + 1) % MAX_QUEUE;
        pool.count--;

        pthread_mutex_unlock(&pool.mutex);
        //执行任务
        (*(task.func))(task.arg);
    }
    return NULL;
}
void threadpool_init()
{
    pool.front = 0;
    pool.rear = 0;
    pool.count = 0;
    pool.shutdown = 0;

    pthread_mutex_init(&pool.mutex, NULL);
    pthread_cond_init(&pool.cond, NULL);

    //创建线程
    for (int i = 0; i < MAX_THREADS;i++)
    {
        pthread_create(&pool.threads[i],NULL,thread_work,NULL);
    }
}

//测试任务
void test_task(void* arg){
    int num = *(int *)arg;
    std::cout<<"线程:"<<pthread_self()<<"执行任务："<<num<<std::endl;
    sleep(1);//模拟耗时任务
}
//添加线程池任务
void threadpool_add(void (*func)(void *),void *arg){
    pthread_mutex_lock(&pool.mutex);

    //队列满
    if(pool.count == MAX_QUEUE){
        pthread_mutex_unlock(&pool.mutex);
        return;
    }
    //加入任务
    pool.queue[pool.rear].func = func;
    pool.queue[pool.rear].arg = arg;
    pool.rear = (pool.rear + 1) % MAX_QUEUE;
    pool.count++;

    pthread_cond_signal(&pool.cond);
    pthread_mutex_unlock(&pool.mutex);
}
//销毁线程池
void threadpool_destroy(){
    if(pool.shutdown)
        return;
    pool.shutdown = 1;
    pthread_cond_broadcast(&pool.cond);

    for (int i = 0; i < MAX_THREADS;i++){
        pthread_join(pool.threads[i], NULL);
    }
    pthread_mutex_destroy(&pool.mutex);
    pthread_cond_destroy(&pool.cond);
}
int main(){
    threadpool_init();
    std::cout << "线程池初始化成功" << std::endl;

    //添加10个任务
    for (int i = 1; i <= 10;i++)
    {
        int* num = (int *)malloc(sizeof(int));
        *num = i;
        threadpool_add(test_task, num);
    }
    sleep(12);
    threadpool_destroy();
    std::cout << "线程池销毁成功" << std::endl;
    return 0;
}