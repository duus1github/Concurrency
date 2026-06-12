#include "rclcpp/rclcpp.hpp"
#include <string>

class ParameterNode:public rclcpp::Node{
public:
    ParameterNode() : Node("param_node"){
        //声明参数+设置默认值
        this->declare_parameter<int>("robot_id", 1);
        this->declare_parameter<std::string>("robot_name", "test_robot");
        this->declare_parameter<double>("speed", 0.5);
        this->declare_parameter<bool>("enable", true);

        //定时器
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&ParameterNode::timer_callback,this)
        );
        RCLCPP_INFO(this->get_logger(),"param_node start");
    }
private:
    void timer_callback(){
        int robot_id = this->get_parameter("robot_id").as_int();
        std::string robot_name = this->get_parameter("robot_name").as_string();
        double speed = this->get_parameter("speed").as_double();
        bool enable = this->get_parameter("enable").as_bool();

        RCLCPP_INFO(this->get_logger(),"ID: %d, name: %s, speed: %f, enable: %d",robot_id,robot_name.c_str(),speed,enable);
    }
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char**argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<ParameterNode>());
    rclcpp::shutdown();
    return 0;
}