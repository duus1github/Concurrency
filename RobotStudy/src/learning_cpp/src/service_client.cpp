#include "rclcpp/rclcpp.hpp"
#include "learning_cpp/srv/add_two_ints.hpp"
#include <chrono>
#include <cstdlib>
#include <memory>

using AddTwoInts = learning_cpp::srv::AddTwoInts;

class AddClient : public rclcpp::Node
{
public:
    AddClient() : Node("cpp_client")
    {
        client_ = this->create_client<AddTwoInts>("/add_two_ints");
        // 等待服务端上线
        while (!client_->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_INFO(this->get_logger(), "等待服务端上线...");
        }
        RCLCPP_INFO(this->get_logger(), "服务端已就绪");
    }

    bool send_req(int64_t a, int64_t b, int64_t& out_sum)
    {
        auto request = std::make_shared<AddTwoInts::Request>();
        request->a = a;
        request->b = b;

        // 异步发送请求（Humble 标准用法）
        auto future = client_->async_send_request(request);
        // 自旋等待请求完成
        auto ret_code = rclcpp::spin_until_future_complete(
            this->get_node_base_interface(), future
        );

        if (ret_code == rclcpp::FutureReturnCode::SUCCESS)
        {
            out_sum = future.get()->sum;
            return true;
        }
        return false;
    }

private:
    rclcpp::Client<AddTwoInts>::SharedPtr client_;
};

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        RCLCPP_ERROR(rclcpp::get_logger("main"), "参数错误，用法：ros2 run learning_cpp 数字1 数字2");
        return 1;
    }

    int64_t num1 = atoll(argv[1]);
    int64_t num2 = atoll(argv[2]);
    int64_t sum = 0;

    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddClient>();

    if (node->send_req(num1, num2, sum))
    {
        RCLCPP_INFO(node->get_logger(), "计算结果：%ld", sum);
    }
    else
    {
        RCLCPP_ERROR(node->get_logger(), "服务调用失败");
    }

    rclcpp::shutdown();
    return 0;
}