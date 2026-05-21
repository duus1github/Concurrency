// 2.读写锁并发读取测试
#include <thread>
#include <vector>
#include <shared_mutex>
#include <mutex>
#include <iostream>

class ReadWriteData{
private:
    std::shared_mutex rw_mtx;
    int data = 0;
public:
    //读共享
    int get(){
        std::shared_lock<std::shared_mutex> lock(rw_mtx);
        return data;
    }
    //写独占
    void set(int v){
        std::unique_lock<std::shared_mutex> lock(rw_mtx);
        data = v;
    }
};
//多线程读
void read_task(ReadWriteData& d,int times){
    volatile int tmp;
    for (int i = 0; i < times;i++)
        tmp = d.get();
}
int main()
{
    ReadWriteData rw_data;
    std::vector<std::thread> read_threads;
    for (int i = 0; i < 8;i++)
    {
        read_threads.emplace_back(read_task,std::ref(rw_data),500000);
    }
    rw_data.set(666);
    for(auto &t:read_threads)
        t.join();
    std::cout<<"多读访问完成"<<std::endl;
}