// 2.线程安全停止(无锁,无竞态)
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> g_stop{false};

void safe_worker(){
    int count = 0;
    while(!g_stop){
        std::cout << "工作中..." << count++ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "线程安全退出" << std::endl;
}

int main(){
    std::thread t(safe_worker);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "发生停止信号" << std::endl;
    g_stop = true;

    t.join();
    return 0;
}