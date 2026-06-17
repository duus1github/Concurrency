#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "sensor_msgs/point_cloud2_iterator.hpp"

class PointCloudSubNode:public rclcpp::Node{
public:
    PointCloudSubNode():Node("pointcloud_sub_node"){
        pc_sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>("points", 10, std::bind(&PointCloudSubNode::pc_callback,this,std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(),"PointCloudSubNode init");
    }
private:
    void pc_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
        //1.获取点云所属坐标系
        std::string pc_frame = msg->header.frame_id;
        // 计算总点数量
        unsigned int total_point = msg->width * msg->height;
        RCLCPP_INFO(this->get_logger(),"点云坐标系:%s, | 点数量:%d",pc_frame.c_str(),total_point);
        
        //2.创建点云迭代器
        sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
        sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
        sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

        //3.仅打印前5个点，防止终端刷屏
        int print_count = 0;
        for (; iter_x != iter_x.end();++iter_x,++iter_y,++iter_z){
            if(print_count >= 5)break;
            float x = *iter_x;
            float y = *iter_y;
            float z = *iter_z;
            RCLCPP_INFO(this->get_logger(),"点[%d] x=%.2f, y=%.2f, z=%.2f",print_count,x,y,z);
            print_count++;
        }
    }
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pc_sub_;
};
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PointCloudSubNode>());
    rclcpp::shutdown();
    return 0;
}