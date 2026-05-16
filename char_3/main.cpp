//4.线程安全栈
#include <iostream>
#include <stack>
#include <mutex>
#include <thread>


template<typename T>
class threadsafe_stack{
private:
    std::stack<T> st;
    mutable std::mutex mtx;
public:
    threadsafe_stack() = default;
    void push(T value){
        std::lock_guard<std::mutex> lock(mtx);
        st.push(value);
    }
    bool pop(T& out){
        std::lock_guard<std::mutex> lock(mtx);
        if(st.empty())
            return false;
        out = std::move(st.top());
        st.pop();
        return true;
    }
    bool empty()const{
        std::lock_guard<std::mutex> lock(mtx);
        return st.empty();
    }
};
int main()
{
    threadsafe_stack<int> st;
    st.push(10);
    st.push(20);

    int val;
    st.pop(val);
    std::cout<<"pop value:"<<val<<std::endl;
    return 0;
}
