#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
//1.基础线程函数
std::mutex print_mtx;//保护输出的互斥锁
void func(int id)
{
    std::lock_guard<std::mutex> lock(print_mtx);
    std::cout<<"Thread:"<<id<<" is running."<<std::endl;
}
int main(){
    //1.启动线程
    std::thread t1(func, 1);
    t1.join();//等待线程结束

    //2.lambda表达式启动线程
    std::thread t2([]
                   { std::lock_guard<std::mutex> lock(print_mtx);
                std::cout<<"lambda 线程运行\n"; });
    t2.join();
}
