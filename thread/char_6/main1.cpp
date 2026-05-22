// 1.线程安全队列（生产者 - 消费者 ）
#include <iostream>
#include <thread>
#include<condition_variable>

#include <queue>
#include <mutex>
#include <chrono>


template<typename T>
class threadsafe_queue{
private:
    std::queue<T>q;
    mutable std::mutex mtx; //允许const成员函数中修改他，可以被lock和unlock,这个是实现线程安全的const成员函数的标准做法
    std::condition_variable cv; //用于线程间的等待与通知，必须配合std::unique_lock<std::mutex>使用


public:
    threadsafe_queue() = default;
    void push(T val){
        std::lock_guard<std::mutex>lock(mtx);
        q.push(val);
        cv.notify_one();
    }
    bool wait_and_pop(T& val){
        std::unique_lock<std::mutex>lock(mtx);
        cv.wait(lock,[this]{return !q.empty();});
        val = std::move(q.front());
        q.pop();
        return true;

    }
    bool try_pop(T &val){
        std::lock_guard<std::mutex>lock(mtx);
        if(q.empty()) return false;
        val = std::move(q.front());
        q.pop();
        return true;
    }
    void empty() const{
        std::lock_guard<std::mutex>lock(mtx);
        return q.empty();
    }
};

void producer(threadsafe_queue<int>& q){
    for(int i=0;i<=5;i++){
        std::cout<<"生产者 ->"<<std::to_string(i) <<std::endl;
        q.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}
void comsumer(threadsafe_queue<int>& q){
    for(int i=0;i<=5;i++){
        int val;
        q.wait_and_pop(val);
        std::cout<<"消费者->"<<std::to_string(val)<<std::endl;
    }
}
int main(){
    std::cout << "生产者 - 消费者"<<std::endl;
    threadsafe_queue<int> q;
    std::thread t1(producer,std::ref(q));
    std::thread t2(comsumer,std::ref(q));
    t1.join();
    t2.join();
    
}