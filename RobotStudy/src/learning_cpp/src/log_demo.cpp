#include "rclcpp/rclcpp.hpp"
#include <chrono>

class LogDemoNode: public rclcpp::Node{
public:
    LogDemoNode():Node("log_demo_node"){
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1),
            std::bind(&LogDemoNode::log_callback,this)
        );
        this->get_logger().set_level(rclcpp::Logger::Level::Debug);
    }
private:
    void log_callback(){
        //日志分级宏
        RCLCPP_DEBUG(this->get_logger(),"debug log");
        RCLCPP_INFO(this->get_logger(),"info log");
        RCLCPP_WARN(this->get_logger(),"warn log");
        RCLCPP_ERROR(this->get_logger(),"error log");
        RCLCPP_FATAL(this->get_logger(),"fatal log");
    }
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<LogDemoNode>());
    
    rclcpp::shutdown();
    return 0;
}