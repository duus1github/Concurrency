#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"

class ImuSubNode : public rclcpp::Node{
public:
    ImuSubNode() :Node("imu_sub_node"){
        imu_sub_ = this->create_subscription<sensor_msgs::msg::Imu>("imu", 10, std::bind(&ImuSubNode::imu_callback, this, std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(), "Imu Sub Node Started, waiting for imu data...");
    }
private:
    void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg){
        //1.获取imu坐标系
        std::string imu_frame = msg->header.frame_id;

        //2.提取四元素
        tf2::Quaternion q(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
        //四元素转欧拉角
        tf2::Matrix3x3 mat(q);
        double roll, pitch, yaw;
        mat.getRPY(roll, pitch, yaw);

        //3.提取角速度，加速度
        double ang_x = msg->angular_velocity.x;
        double ang_y = msg->linear_acceleration.y;

        RCLCPP_INFO(this->get_logger(), "坐标系: %s roll: %f, pitch: %f, yaw: %f, 角速度: %f, 加速度: %f",
        imu_frame.c_str(), roll, pitch, yaw,ang_x,ang_y);
    }
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<ImuSubNode>());
    rclcpp::shutdown();
    return 0;
}