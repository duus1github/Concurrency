#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include <chrono>
#include <string>

using namespace std::chrono_literals;

class TfListener : public rclcpp::Node{
public:
    TfListener():Node("tf_listener_node"){
        //缓存：存储坐标变换，缓存时长3秒
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        //坐标监听器
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        timer_ = this->create_wall_timer(500ms,std::bind(&TfListener::listen_tf,this));
    }
private:
    void listen_tf(){
        geometry_msgs::msg::TransformStamped t;
        t = tf_buffer_->lookupTransform("base_link","pan_link",tf2::TimePointZero);
        //提取平移
        double x = t.transform.translation.x;
        double y = t.transform.translation.y;
        double z = t.transform.translation.z;

        //四元素转欧拉角
        tf2::Quaternion q(
            t.transform.rotation.x,t.transform.rotation.y,t.transform.rotation.z,t.transform.rotation.w
        );
        tf2::Matrix3x3 mat(q);
        double roll, pitch, yaw;
        mat.getRPY(roll, pitch, yaw);

        RCLCPP_INFO(this->get_logger(), "平移:x=%.2f,y=%.2f,z=%.2f | 姿态: roll=%.2f,pitch=%.2f,yaw=%.2f",
            x,y,z,roll,pitch,yaw);
    }
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfListener>());
  rclcpp::shutdown();
  return 0;
}