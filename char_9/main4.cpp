// 4.一次性初始化(多线程安全只执行一次)

#include <iostream>
#include <thread>
#include <mutex>

std::once_flag g_init_flag;
void init_resource(){
    std::cout << "===全局变量初始化(只执行1次)===" << std::endl;
}
void task(){
    std::call_once(g_init_flag,init_resource);
    std::cout << "线程运行\n" << std::endl;
}
int main(){
    std::thread t1(task);
    std::thread t2(task);
    std::thread t3(task);

    t1.join(), t2.join(), t3.join();
    return 0;
}