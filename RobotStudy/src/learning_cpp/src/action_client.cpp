#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "learning_cpp/action/progress.hpp"
#include <functional>

using Progress = learning_cpp::action::Progress;
using GoalHandleProgress = rclcpp_action::ClientGoalHandle<Progress>;

class ProgressClient:public rclcpp::Node{
public:
    ProgressClient():Node("progress_client"){
        action_client_ = rclcpp_action::create_client<Progress>(this, "progress_action");

    }
    //发送倒计时
    void send_goal(int total_sec){
        if(!action_client_->wait_for_action_server(std::chrono::seconds(5))){
            RCLCPP_INFO(this->get_logger(),"Action 服务端未上线！");
            return;
        }
        //构造目标
        auto goal_msg = Progress::Goal();
        goal_msg.total_sec = total_sec;
        RCLCPP_INFO(this->get_logger(),"发送倒计时目标：%d秒",total_sec);

        //发送目标，绑定各类回调
        auto send_goal_options = rclcpp_action::Client<Progress>::SendGoalOptions();
        send_goal_options.goal_response_callback = std::bind(&ProgressClient::goal_response_callback,this,std::placeholders::_1);
        send_goal_options.feedback_callback = std::bind(&ProgressClient::feedback_callback,this,std::placeholders::_1,std::placeholders::_2);
        send_goal_options.result_callback = std::bind(&ProgressClient::result_callback,this,std::placeholders::_1);

        action_client_->async_send_goal(goal_msg, send_goal_options);
    }
private:
    void goal_response_callback(const GoalHandleProgress::SharedPtr &goal_handle){
        if(!goal_handle){
            RCLCPP_ERROR(this->get_logger(),"Goal was rejected by server");
        }else{
            RCLCPP_INFO(this->get_logger(),"目标已接受，开始执行");
        }
    }
    //实时反馈回调（接收剩余时间）
    void feedback_callback(GoalHandleProgress::SharedPtr,const std::shared_ptr<const Progress::Feedback>feedback){
        RCLCPP_INFO(this->get_logger(),"客户端收到反馈：剩余%d秒",feedback->left_sec);
    }
    //最终结果回调（任务结束）
    void result_callback(const GoalHandleProgress::WrappedResult &result){
        switch(result.code){
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(),"任务成功完成");
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_INFO(this->get_logger(),"任务被取消");
                break;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_INFO(this->get_logger(),"任务被取消");
            default:
                RCLCPP_ERROR(this->get_logger(),"未知错误");
        }
        rclcpp::shutdown();
    }
    rclcpp_action::Client<Progress>::SharedPtr action_client_;
};

int main(int argc,char *argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ProgressClient>();
    node->send_goal(5);
    rclcpp::spin(node);
    return 0;
}