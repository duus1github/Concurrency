#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listener:public rclcpp::Node{
public:
    Listener():Node("listener"){
        sub_ = this->create_subscription<std_msgs::msg::String>("/chatter", 10,
                std::bind(&Listener::topic_cb, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(),"c++ 订阅话题节点启动");
    }
private:
    void topic_cb(const std_msgs::msg::String::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(),"recv: %s",msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};
int main(int argc, char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<Listener>());
    rclcpp::shutdown();
    return 0;
}