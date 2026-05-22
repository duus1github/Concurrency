// 综合案例,工业级工作线程(绑定CPU,可安全停止,高性能)

#include <iostream>
#include <thread>
#include <atomic>
#include <pthread.h>

class Worker{
    private:
        std::thread m_thread;
        std::atomic<bool> m_running{false};
        std::atomic<bool> m_stop{false};
        int m_core_id;

        void set_affinity(){
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(m_core_id,&cpuset);
            pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
        }

        void run(){
            set_affinity();
            m_running = true;
            std::cout << "worker 线程已启动,绑定core:" << m_core_id << std::endl;

            while(!m_stop){
                std::cout << "worker 工作中..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            m_running = false;
            std::cout << "worker 线程安全退出..." << std::endl;
        }

    public:
        Worker(int core_id):m_core_id(core_id){}

        void start(){
            m_thread = std::thread(&Worker::run, this);
        }
        void stop(){
            m_stop = true;
            if(m_thread.joinable()){
                m_thread.join();
            }
        }
        bool is_runnning(){
            return m_running;
        }
};
int main(){
    Worker wt(0);
    wt.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "发生停止命令" << std::endl;
    wt.stop();

    return 0;
}