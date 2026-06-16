#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"

class StaticTfPub:public rclcpp::Node{
public:
    StaticTfPub():Node("static_tf_pub"){
        static_broadcaster = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        public_tf();
        RCLCPP_INFO(this->get_logger(),"Static TF Publisher Started... base_link ->laser_link");
    }
private:
    void public_tf(){
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->get_clock()->now();
        transform.header.frame_id = "base_link";
        transform.child_frame_id = "laser_link";

        //平移
        transform.transform.translation.x = 0.3;
        transform.transform.translation.y = 0.0;
        transform.transform.translation.z = 0.15;

        //欧拉角转四元素
        tf2::Quaternion q;
        q.setRPY(0,0,0);
        transform.transform.rotation.x = q.x();
        transform.transform.rotation.y = q.y();
        transform.transform.rotation.z = q.z();
        transform.transform.rotation.w = q.w();

        static_broadcaster->sendTransform(transform);
    }
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_broadcaster;
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<StaticTfPub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}