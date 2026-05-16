// 4.std::packaged_task 包装可调用对象
#include <iostream>
#include <thread>
#include <mutex>
#include <future>

int func_package(int a,int b){
    return a + b;
}
int main(){
    std::packaged_task<int(int, int)> task(func_package);
    std::future<int> fut_task = task.get_future();
    task(10, 20);
    std::cout<<"packaged_task res ... "<<std::to_string(fut_task.get())<<std::endl;
    return 0;
}