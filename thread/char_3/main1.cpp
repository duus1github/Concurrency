// 1.lock_guard 实现线程安全的共享变量累加

#include <iostream>
#include <thread>
#include <mutex>

int share_count = 0;
std::mutex count_mtx;
std::mutex print_mtx;
void add_count(int n)
{
    for (int i = 0; i < 100000;i++){
        //自动加锁,解锁
        std::lock_guard<std::mutex> lock(count_mtx);
        share_count++;
    }
    std::lock_guard<std::mutex> print_lock(print_mtx);
    std::cout << "thread " << n << " success \n" << std::endl;
}
int main(){
    // 1.共享变量安全累加
    std::thread t1(add_count, 1);
    std::thread t2(add_count, 2);
    t1.join();
    t2.join();
    std::cout << "final count:" << share_count << std::endl;
}