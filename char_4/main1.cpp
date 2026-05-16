// 1.生产者-消费者
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

// 条件变量+线程安全队列(生产者-消费者模型)
template<typename T>
class threadsafe_queue{
private:
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cv;
public:
    void push(T val){
        std::lock_guard<std::mutex> lock(mtx);
        q.push(std::move(val));
        cv.notify_one();//唤醒一个等待线程
    }
    void wait_and_pop(T &out){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]()
                { return !q.empty(); });
        out = std::move(q.front());
        q.pop();
    }
};
//生产者
void producer(threadsafe_queue<int> &q){
    for (int i = 1; i <= 5;i++)
    {
        std::cout<<"生产者生产了:"<<i<<std::endl;
        q.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
// 消费者
void consumer(threadsafe_queue<int> &q){
    for (int i = 1; i <= 5;i++){
        int val;
        q.wait_and_pop(val);
        std::cout<<"消费者消费了:"<<val<<std::endl;
    }
}
int main()
{
    std::cout << "生产者-消费者 start ..." << std::endl;
    threadsafe_queue<int> q;
    std::thread t_producer(producer, std::ref(q));
    std::thread t_consumer(consumer, std::ref(q));
    t_producer.join();
    t_consumer.join();
    std::cout<<"生产者-消费者 end ..." << std::endl;
}