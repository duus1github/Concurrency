// 2.async 异步任务,已经执行了,会把执行结果放在future对象中,然后通过get()获取结果,如果任务未完成,则会阻塞等待任务完成

#include <iostream>
#include <thread>
#include <mutex>
#include <future>
int async_task(int x){
    std::cout<<"async task start ...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    return x * 10;
}

int main(){
    std::future<int> fut = std::async(std::launch::async,async_task,10);
    std::cout<<"async task res ..."<<std::to_string(fut.get())<<std::endl;
}