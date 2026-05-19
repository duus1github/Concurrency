// 2、线程池，（第6章核心工程）
#include<iostream>
#include <vector>
#include <future>
#include <chrono>
#include <thread>
#include <atomic>
#include <queue>

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


class thread_pool{
private:
    using task = std::function<void()>;
    std::vector<std::thread> threads;
    threadsafe_queue<task>queue;
    std::atomic<bool>stop{false};

    void worker(){
        while(!stop){
            task t;
            if(queue.try_pop(t)){
                t();
            }else{
                std::this_thread::yield(); //线程中的函数，作用是让当前线程主动让出CPU时间片
            }
        }
    }
public:
    thread_pool(){
        auto n = std::thread::hardware_concurrency();
        for(unsigned i = 0;i<n;i++){
            threads.emplace_back(&thread_pool::worker,this);
        }

    }
    ~thread_pool(){
        stop = true;
        for(auto &t:threads)if(t.joinable())t.join();
    }
    template<typename F>
    auto submit(F&& f)->std::future<decltype(f())>{
        using R = decltype(f()); //decltype关键字：不会真调用f(),只是编译期推导f调用后的返回类型，作用是让线程池支持任何返回类型
        //packaged_task<R()>是一个任务包装器，调用它->执行函数->结果自动写入promise->可通过future获取;作用：让普通函数变成可异步，可获取返回值的任务。
        auto func = std::make_shared<std::packaged_task<R()>>(std::forward<F>(f));
        //从package_task中取出未来结果
        std::future<R> res = func->get_future();
        //把任务包装成lambda,放入线程池
        queue.push([func](){(*func)();});
        return res;

    }
};

int main(){
    std::cout<<"线程池..."<<std::endl;
    thread_pool pool;
    std::vector<std::future<int>>futs;
    
    for(int i=1;i<=4;i++){
        futs.push_back(pool.submit([i]{
            std::cout<<"线程池任务："<<std::to_string(i)<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return i*10;
        }));
    }
    for (auto &f:futs) std::cout<<"res:"<<std::to_string(f.get())<<std::endl;
}