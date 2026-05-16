// 2.CAS无锁栈(<c++并发编程实战>(经典))
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>


template<typename T>
class lock_free_stack{
private:
    struct Node{
        T data;
        Node *next;
        Node(T val): data(val),next(nullptr) {}
    };
    std::atomic<Node *> head{nullptr};
public:
    void push(T val){
        Node *new_node = new Node(val);
        new_node->next = head.load(std::memory_order_relaxed);
        while(!head.compare_exchange_weak(new_node->next,new_node,
            std::memory_order_release,std::memory_order_relaxed)){

        }
    }
    bool pop(T &out){
        Node *old_head = head.load(std::memory_order_relaxed);
        while(old_head != nullptr){
            if(head.compare_exchange_weak(
                old_head,old_head->next,
                std::memory_order_acquire,std::memory_order_relaxed
            )){
                out = old_head->data;
                delete old_head;
                return true;
            }
        }
        return false;
    }
};

int main(){
    std::cout << "无锁栈 push/pop" << std::endl;
    lock_free_stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    int val;
    while(stack.pop(val)){
        std::cout<<"pop: "<<val<<std::endl;
    }
}