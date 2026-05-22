// 3.线程本地存储TLS
#include <iostream>
#include <thread>

//线程本地变量:每个线程独立一份
thread_local int tls_counter = 0;
void task(int id){
    tls_counter++;
    std::cout << "线程:" << id << " counter:" << tls_counter << std::endl;
}
int main(){
    std::thread t1(task, 1);
    std::thread t2(task, 2);
    t1.join();
    t2.join();
    return 0;
}