#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "learning_cpp/action/progress.hpp"
#include <chrono>
#include <thread>

using Progress = learning_cpp::action::Progress;
using GoalHandleProgress = rclcpp_action::ServerGoalHandle<Progress>;

class ProgressActionServer:public rclcpp::Node{
public:
    ProgressActionServer():Node("progress_action_server"){
        action_server_ = rclcpp_action::create_server<Progress>(
            this, "progress_action",
            std::bind(&ProgressActionServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&ProgressActionServer::handle_cancel, this, std::placeholders::_1),
            std::bind(&ProgressActionServer::handle_accepted, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(),"Action server started");
    }
private:
    //处理客户端下发的目标
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid,std::shared_ptr<const Progress::Goal>goal){
        RCLCPP_INFO(this->get_logger(),"收到倒计时目标，总时长：%d秒",goal->total_sec);
        (void)uuid;
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    //处理取消请求
    rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleProgress>goal_handle){
        RCLCPP_INFO(this->get_logger(), "收到取消请求");
        (void)goal_handle;
        return rclcpp_action::CancelResponse::ACCEPT;
    }
    //接受目标后，开启任务执行线程
    void handle_accepted(const std::shared_ptr<GoalHandleProgress>goal_handle){
        std::thread{std::bind(&ProgressActionServer::execute, this, goal_handle)}.detach();
    }
    //核心任务：倒计时逻辑
    void execute(const std::shared_ptr<GoalHandleProgress>goal_handle){
        RCLCPP_INFO(this->get_logger(), "开始执行倒计时任务...");
        const auto goal = goal_handle->get_goal();
        auto feedback = std::make_shared<Progress::Feedback>();
        auto result = std::make_shared<Progress::Result>();

        int total = goal->total_sec;
        for (int i = total; i > 0;i--)
        {
            if(goal_handle->is_canceling()){
                result->success = false;
                goal_handle->canceled(result);
                RCLCPP_INFO(this->get_logger(),"任务取消");
                return;
            }
            //填充反馈，并发送
            feedback->left_sec = i;
            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(),"剩余时间：%d秒",feedback->left_sec);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        //任务正常完成
        result->success = true;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(),"倒计时任务完成");
    }
    rclcpp_action::Server<Progress>::SharedPtr action_server_;
};

int main(int argc,char *argv[]){
    rclcpp::init(argc,argv);
    auto node = std::make_shared<ProgressActionServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
