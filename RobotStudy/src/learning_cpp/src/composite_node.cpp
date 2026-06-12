#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int64.hpp"
#include "learning_cpp/srv/add_two_ints.hpp"
#include <chrono>

using AddTwoInts = learning_cpp::srv::AddTwoInts;
using Int64Msg = std_msgs::msg::Int64;

class CompositeNode : public rclcpp::Node
{
public:
    CompositeNode()
    : Node("composite_node"), count_(0)
    {
        // ========== 1. 声明并读取参数 ==========
        this->declare_parameter<int>("pub_freq", 1);    // 发布频率(Hz)
        this->declare_parameter<int>("base_num", 0);    // 初始计数值
        int freq = this->get_parameter("pub_freq").as_int();
        count_ = this->get_parameter("base_num").as_int();

        RCLCPP_INFO(this->get_logger(), "综合节点启动");
        RCLCPP_INFO(this->get_logger(), "发布频率：%d Hz, 初始值：%d", freq, count_);

        // ========== 2. 创建话题发布者 ==========
        publisher_ = this->create_publisher<Int64Msg>("data_topic", 10);

        // ========== 3. 创建定时器（按参数频率发布） ==========
        auto period = std::chrono::milliseconds(1000 / freq);
        timer_ = this->create_wall_timer(
            period,
            std::bind(&CompositeNode::pub_callback, this)
        );

        // ========== 4. 创建服务端 ==========
        service_ = this->create_service<AddTwoInts>(
            "add_service",
            std::bind(&CompositeNode::service_callback, this,
                      std::placeholders::_1, std::placeholders::_2)
        );
        RCLCPP_INFO(this->get_logger(), "服务 [add_service] 已就绪");
    }

private:
    // 话题发布回调
    void pub_callback()
    {
        count_++;
        auto msg = Int64Msg();
        msg.data = count_;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "发布数据：%ld", msg.data);
    }

    // 服务回调：接收两个数相加，同时更新计数值
    void service_callback(
        const std::shared_ptr<AddTwoInts::Request> req,
        std::shared_ptr<AddTwoInts::Response> res)
    {
        res->sum = req->a + req->b;
        count_ = res->sum;
        RCLCPP_WARN(this->get_logger(),
                    "收到服务请求: %ld + %ld = %ld, 计数值已更新",
                    req->a, req->b, res->sum);
    }

    rclcpp::Publisher<Int64Msg>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Service<AddTwoInts>::SharedPtr service_;
    int count_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CompositeNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}