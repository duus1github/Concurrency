// 1、伪共享问题+ 内存对齐解决
#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
using Clock = std::chrono::high_resolution_clock;
using Ms = std::chrono::milliseconds;


// 未对齐，极易产生伪共享
struct NoAlignData{
    std::atomic<long long>cnt1{0};
    std::atomic<long long>cnt2{0};
};
//缓存行对齐：彻底隔离，消除伪缓存
struct alignas(std::hardware_destructive_interference_size) AlignCounter{
    std::atomic<long long>val{0};
};
// 伪共享压测函数
void inc_counter(AlignCounter& obj,int loop){
    for(int i=0;i<loop;i++){
        obj.val.fetch_add(1,std::memory_order_relaxed);
    }
}
int main(){
    std::cout<<"伪共享优化对比测试"<<std::endl;
    AlignCounter c1,c2;

    auto t1 = Clock::now();
    std::thread t_a(inc_counter,std::ref(c1),1000000);
    std::thread t_b(inc_counter,std::ref(c2),1000000);
    t_a.join(),t_b.join();

    auto t2 = Clock::now();
    std::cout<<"对齐后消耗"<<std::chrono::duration_cast<Ms>(t2-t1).count()<<"ms"<<std::endl;
}