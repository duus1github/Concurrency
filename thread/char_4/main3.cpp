// 3.promise + future 手动传值
#include <iostream>
#include <thread>
#include <mutex>
#include <future>

void promise_task(std::promise<int> p){
    std::cout<<"promise task start ...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    p.set_value(100);
}

int main(){
    std::promise<int> p;
    std::future<int> fut_p = p.get_future();
    std::thread t_promise(promise_task,std::move(p));
    std::cout<<"promise task res ..."<<std::to_string(fut_p.get())<<std::endl;
    t_promise.join();
}