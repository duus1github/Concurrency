// 4.宽松内存序高并发计数,仅保证原子,不保证顺序
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
std::atomic<long> relaxed_counter{0};

void relaxed_increment(){
    for (int i = 0; i < 100000;i++){
        relaxed_counter.fetch_add(1, std::memory_order_relaxed);
    }
}
int main()
{
    std::cout << "宽松内存序高并发计数" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 10;i++)
    {
        threads.emplace_back(relaxed_increment);
    }
    for(auto &t:threads)
        t.join();

    std::cout << "最终计数:" << relaxed_counter << std::endl;
}