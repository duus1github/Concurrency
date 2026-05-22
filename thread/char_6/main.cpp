// 4.流水线模式，
#include <thread>
#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>

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


void stage1(threadsafe_queue<int>& in,threadsafe_queue<int>& out){
    int val;
    while(in.wait_and_pop(val)){
        std::cout<<"阶段1：处理"<<std::to_string(val)<<std::endl;
        out.push(val*2);
    }

}
void stage2(threadsafe_queue<int>& in,threadsafe_queue<int>& out){
    int val;
    while(in.wait_and_pop(val)){
        std::cout<<"阶段2：处理"<<std::to_string(val)<<std::endl;
        out.push(val+10);
    }
}

void stage3(threadsafe_queue<int>& in){
    int val;
    while(in.wait_and_pop(val)){
        std::cout<<"阶段3，最终输出："<<std::to_string(val)<<std::endl;

    }
}
int main(){
    std::cout<<"流水线：..."<<std::endl;
    threadsafe_queue<int>q1,q2,q3;
    std::thread s1(stage1,std::ref(q1),std::ref(q2));
    std::thread s2(stage2,std::ref(q2),std::ref(q3));
    std::thread s3(stage3,std::ref(q3));

    for(int i=0;i<=3;i++) q1.push(i);
    q1.push(-1);q2.push(-1);q3.push(-1);

    s1.join();s2.join();s3.join();

    std::cout<<"第六章演示结束"<<std::endl;
}   