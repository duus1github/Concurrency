// 3.Release-Acquire 同步内存序同步(核心机制)
#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>

std::atomic<int> data{0};
std::atomic<bool> ready{false};

void producer_release(){
    data.store(10240,std::memory_order_relaxed);
    //写屏障,此前所有操作对acquire端可见
    ready.store(true,std::memory_order_release);
}
void consumer_acquire(){
    //读屏障,确保看到release之前的全部写入
    while(!ready.load(std::memory_order_acquire)){
        //等待生产者准备好数据
        std::this_thread::yield(); //让出CPU时间片

    }
    //此时可以安全读取data
    // assert(data.load(std::memory_order_relaxed) == 1024);
    data.load(std::memory_order_relaxed);
    std::cout << "consumer acquire read data:" << data << std::endl;
}
int main(){
    std::cout << "release-acquire demo" << std::endl;
    std::thread t1(producer_release);
    std::thread t2(consumer_acquire);
    t1.join();
    t2.join();
}