// 1、
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
//====基础无锁栈
template<typename T>
class LockFreeStack{
private:
    struct Node{
        T data;
        Node* next;
        Node(T val):data(std::move(val)),next(nullptr){}
    };
    std::atomic<Node *>head_{nullptr};
public:
    void push(T val){
        Node* new_node = new Node(std::move(val));
        Node* old_head = head_.load(std::memory_order_relaxed);
        do{
            new_node->next = old_head;
            //CAS更新头节点，失败自动更新old_head
        }while(!head_.compare_exchange_weak(old_head,new_node,std::memory_order_release,std::memory_order_relaxed));
    }
    bool pop(T& out_val){
        Node *old_head = head_.load(std::memory_order_acquire);
        while(old_head != nullptr){
            Node* next_node = old_head->next;
            if(head_.compare_exchange_weak(old_head,next_node,std::memory_order_acquire,std::memory_order_relaxed)){
                out_val = std::move(old_head->data);
                delete old_head;
                return true;
            }

        }
        return false;
    }
    ~ LockFreeStack(){
        Node *cur = head_.load();
        while(cur){
            Node* del = cur;
            cur = cur->next;
            delete del;
        }
    }
};

void stack_producer(LockFreeStack<int>& st,int id,int cnt){
    for(int i=0;i<=cnt;i++){
        st.push(id*100+i);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    std::cout<<"生产者线程："<<std::to_string(id)<<"完成入栈"<<std::endl;
}

void stack_consumer(LockFreeStack<int>& st,int id,int cnt){
    int val;
    int get_cnt = 0;

    while(get_cnt<cnt){
        
        if(st.pop(val)){
            get_cnt++;
            std::cout<<"消费者线程:"<<std::to_string(id)<<"取出数据："<<std::to_string(val)<<std::endl;
        }

    }
    std::cout<<"消费者线程："<<std::to_string(id)<<"消费完成"<<std::endl;
}
int main(){
    std::cout<<"===无锁栈并发测试==="<<std::endl;
    LockFreeStack<int>lf_stack;
    
    std::vector<std::thread>producers;
    std::vector<std::thread>consumers;

    //启动4个生产者
    for(int i=0;i<=4;i++){
        producers.emplace_back(stack_producer,std::ref(lf_stack),i,20);
    }
    //启动2个消费者
    for(int i=0;i<=2;i++){
        consumers.emplace_back(stack_consumer,std::ref(lf_stack),i,40);
    }
    for(auto& t:producers)t.join();
    for(auto& t:producers)t.join();

    std::cout<<"无锁并法发测试结束"<<std::endl;
    return 0;
}