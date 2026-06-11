#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

using namespace std::chrono_literals;

class Talker :public rclcpp::Node{
public:
    Talker():Node("talker"){
        pub_ = this->create_publisher<std_msgs::msg::String>("/chatter", 10);
        timer_ = this->create_wall_timer(
            1s,std::bind(&Talker::timer_cb,this)
        );
        RCLCPP_INFO(this->get_logger(), "Talker node has been started");
        count = 0;
    }
private:
    void timer_cb(){
        std_msgs::msg::String msg;
        msg.data = "hello c++ topic " + std::to_string(count);
        pub_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "publish: %s", msg.data.c_str());
        count++;
    }
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count;
};
int main(int argc,char *argv[]){
    rclcpp::init(argc,argv);
    auto node = std::make_shared<Talker>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}