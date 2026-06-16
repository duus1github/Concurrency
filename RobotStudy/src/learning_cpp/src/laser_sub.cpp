#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LaserSubNode:public rclcpp::Node{
public:
    LaserSubNode (): Node("laser_sub_node"){
        laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("scan",10,std::bind(&LaserSubNode::laser_callback,this,std::placeholders::_1));
        RCLCPP_INFO(this->get_logger(),"laser_sub_node init done!");
    }
private:
    void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
        //1.获取雷达坐标系(对应TF的laser_link)
        std::string frame = msg->header.frame_id;
        //2.正前方测距索引：数组中间位置
        int front_index = msg->ranges.size() / 2;
        float front_dist = msg->ranges[front_index];

        //过滤雷达无效值
        float valid_dist = front_dist;
        if(front_dist < msg->range_min || front_dist > msg->range_max){
            valid_dist = -1;
        }
        RCLCPP_INFO(this->get_logger(),"laser 坐标系：%s | 正前方距离：%.2f m",frame.c_str(),valid_dist);
    }
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<LaserSubNode>());
    rclcpp::shutdown();
    return 0;
}