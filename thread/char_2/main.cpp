// 3.RAII 线程守卫
#include <iostream>
#include <thread>
#include <mutex>
void func(int x){
    std::cout<<"thread "<<pthread_self() <<" x = "<<x<<std::endl;
}

class thread_guard{
    private:
        std ::thread &t;
    public:
        explicit thread_guard(std::thread& t_):t(t_){}
        //explicit:静止隐式转换,如编译器在初始化的时候将t(t_) 变成为 t = t_,然后就必须使用直接初始化(括号或花括号),防止编译器将一个std::thread对象隐式转换为thread_guard对象 
        ~thread_guard(){
            if(t.joinable()){
                t.join();
                std::cout << "自动 join 线程\n";
            }
        }
        //禁止拷贝
        thread_guard(const thread_guard &) = delete;
        thread_guard &operator=(const thread_guard &) = delete;
};
int main()
{
    std::thread t6(func, 6);
    thread_guard g(t6);

    std::cout << "cpu core num = " << std::thread::hardware_concurrency() << std::endl;
    return 0;
}