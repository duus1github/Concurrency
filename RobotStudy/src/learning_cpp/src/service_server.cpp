#include "rclcpp/rclcpp.hpp"
#include "learning_cpp/srv/AddTwo"

using AddTwoInts = learning_cpp::srv::AddTwoInts;

class AddServer:public rclcpp::Node{
public:
    AddServer():Node("cpp_server"){
        service_ = this->create_service<AddTwoInts>(
            "/add_two_ints",std::bind(&AddServer::server_cb,this,std::placeholders::_1,std::placeholders::_2)
        );
        RCPCPP_INFO(this->get_logger(),"c++ 加法服务端启动");
    }

private:
    void service_cb(const std::shared_ptr<AddTwoInts::Request>req,std::shared_ptr<AddTwoInts::Response>res){
        res->sum = req->a + req->b;
        RCLCPP_INFO(this->get_logger(), "收到 a:%ld b: %ld", req->a, req->b);
        RCLCPP_INFO(this->get_logger(), "发送结果: %ld", res->sum);
    }
    rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc,int argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<AddServer>());
    rclcpp::shutdown();
}