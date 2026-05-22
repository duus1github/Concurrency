#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>

class ThreadPool{
public:
    ThreadPool(int threadCount):stopFlag(false){
        for (int i = 0; i < threadCount;i++){
            workerList.emplace_back([this](){
                while(true){
                    std::function<void()> task;
                    // 加锁访问任务队列
                    std::unique_lock<std::mutex> lock(queueMtx);
                    //条件等待:有任务或线程停止才唤醒
                    taskCond.wait(lock, [this]()
                                  { return !taskQueue.empty() || stopFlag; });
                    //停止且无剩余任务,线程退出
                    if(stopFlag && taskQueue.empty()){
                        return;
                    }
                    //取出队首任务
                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                    //解锁,其他线程可以继续任务
                    lock.unlock();
                    //执行任务
                    task();
                }
            });
        }
        
    }
    //向队列提交任务
    void submitTask(std::function<void()>task){
        std::lock_guard<std::mutex> lock(queueMtx);
        taskQueue.push(std::move(task));
        taskCond.notify_one(); //唤醒一个等待线程处理任务
    }
    //析构函数:安全关闭线程池
    ~ThreadPool(){
        {
            std::lock_guard<std::mutex> lock(queueMtx);
            stopFlag = true;
        }
        //唤醒所有等待线程
        taskCond.notify_all();
        for(auto& t:workerList){
            if(t.joinable()){
                t.join();
            }
        }
    }

private:
    std::vector<std::thread> workerList;        //工作线程集合
    std::queue<std::function<void()>> taskQueue;//任务队列
    std::mutex queueMtx;                        //队列访问锁
    std::condition_variable taskCond;           //任务唤醒条件变量
    bool stopFlag;                                //停止标志
};

int main(){
    ThreadPool pool(4);
    for (int i = 0; i < 8;i++)
    {
        pool.submitTask([i]()
        { 
            std::cout << "task " << i << " 开始执行,线程id:" << std::this_thread::get_id() << std ::endl; 
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout<<"任务 "<<i<< "执行完毕"<<std::endl; 
        });
    }
}