#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <thread>


class HelloNode :public rclcpp::Node{
public:
    HelloNode():Node("hello_node"){
        RCLCPP_INFO(this->get_logger(),"c++ hello node start");
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500), std::bind(&HelloNode::timer_callback, this));
    }
private:
    // 定时器回调函数
    void timer_callback(){
        RCLCPP_INFO(this->get_logger(),"hello ros2 c++");
    }
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc,char *argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<HelloNode>();
    rclcpp::spin(node); //阻塞循环，定时器回调
    rclcpp::shutdown();
    return 0;
}