// 1.unique_lock 灵活锁
#include <iostream>
#include <thread>
#include <mutex>

void unique_lock_demo(){
    std::mutex mtx;
    int shared_data = 0;
    //1. 先创建,不上锁
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    std::cout << "unique_lock 已创建,但未上锁" << std::endl;
    // std::cout << "unique_lock 执行无所安全代码\n";
    lock.lock();
    std::cout << "unique_lock 已手动上锁,进入临界区\n";
    shared_data = 100;
    lock.unlock();
    std::cout << "unique_lock 已手动解锁,离开临界区\n";
    //2. 尝试上锁 try_to_lock
    std::unique_lock<std::mutex> try_lock(mtx, std::try_to_lock);
    if(try_lock.owns_lock()){
        std::cout <<"unique_lock try_to_lock 成功上锁\n";

    }else{
        std::cout<<"unique_lock try_to_lock 上锁失败\n";
    }
    //3.锁所有权转移
    std::unique_lock<std::mutex> lock2 = std::move(try_lock);
    if(lock2){
        std::cout<<"unique_lock 锁所有权转移成功\n";
    }
}
int main(){
    std::cout << "unique_lock start ..." << std::endl;
    unique_lock_demo();
    std::cout<<"unique_lock end ..." << std::endl;
}