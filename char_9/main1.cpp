// 1.线程绑定cpu核心
#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>


//绑定线程到指定CPU核心
void set_thread_affinity(int core_id){
    pthread_t pthread_id = pthread_self();
    //2.创建cpu集合
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);          //清空集合
    CPU_SET(core_id, &cpuset);  //将目标核心加入集合

    //3.执行绑定(系统调用)
    int ret = pthread_setaffinity_np(pthread_id, sizeof(cpu_set_t), &cpuset);
    if(ret!=0){
        std::cerr << "绑核cpu失败" << std::endl;
    }else{
        std::cout << "线程已绑定到cpu core:" << core_id << std::endl;
    }
}
void worker(int core,int id){
    set_thread_affinity(core);
    while(1){
        std::cout << "线程:" << id << ",运行中" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
}

int main(){
    std::thread t1(worker, 5, 1);
    std::thread t2(worker, 8, 2);
    t1.join();
    t2.join();
    return 0;
}