
//3.死锁加安全加锁
#include <thread>
#include <mutex>
#include <iostream>

std::mutex m1, m2;
void deadlock_safe()
{
    //同时锁 m1,m2,不会死锁
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lock1(m1,std::adopt_lock);
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
    std::cout <<"deadlock_safe: success\n";
}
int main(){
    deadlock_safe();
    std::cout << "\n";
}