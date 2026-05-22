//3.带引用的参数函数

#include<iostream>
#include <thread>
#include <mutex>
#include <vector>
void func_ref(int &x){
    x = 999;
}
int main(){
    int x = 0;
    std::thread t3(func_ref, std::ref(x));
    t3.join();
    std::cout<<"thread "<<pthread_self() <<" x = "<<x<<std::endl;

    //4.线程所有权移动
    std::thread t4(func_ref, std::ref(x));
    std::thread t5 = std ::move(t4);
    t5.join();
    std::cout<<"thread "<< pthread_self() <<" x = "<<x<<std::endl;

    //5.批量线程(vector)
    std::vector<std::thread> threads;
    for (int i = 10; i <= 13;++i){
        x+=i;
        threads.emplace_back(func_ref, std::ref(x));
    }
    for(auto &t:threads){
        t.join();
        std::cout<<"thread "<< pthread_self() <<" x = "<<x<<std::endl;
    }
}