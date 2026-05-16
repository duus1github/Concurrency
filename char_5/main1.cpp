// 1.原子操作
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

void atomic_basic_demo(){
    std::atomic<int> a{0};
    //存储
    a.store(10, std::memory_order_relaxed);
    //加载
    int v = a.load(std::memory_order_relaxed);
    std::cout<<"v="<<v<<std::endl;

    //交换
    int old = a.exchange(20);
    std::cout << "exchange old: " << old << " new: "<<a.load() << std::endl;

    //算术原子操作
    a.fetch_add(5);
    std::cout<<"fetch_add 5 =>: "<<a.load()<<std::endl;
    a.fetch_sub(3);
    std::cout<<"fetch_sub 3 =>: "<<a.load()<<std::endl;
}
int main(){
    std::cout<<"automic operations"<<std::endl;
    atomic_basic_demo();
}