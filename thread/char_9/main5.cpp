// 5.可分离线程(后台运行,自动释放)

#include <iostream>
#include <thread>

void daemon_task(){
    std::cout << "后台线程运行" << std::endl;
    std::this_thread ::sleep_for(std::chrono::seconds(2));
    std::cout << "后台线程结束运行" << std::endl;
}
int main(){
    std::thread t(daemon_task);
    t.detach();

    std::cout << "主线程继续运行" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}