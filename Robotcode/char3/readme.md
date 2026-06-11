# 《ros2 智能机器人开发实践》
## 
### 实操命令
- 创建功能模块包,在src下执行：ros2 pkg create --build-type ament_cmake learning_cpp
- 编译功能包：
    colcon build 编译全部的包
    colcon build --select-pkg-name learning_cpp 仅编译learning_cpp包
## 2.4 节点（node）开发
### 核心概念
#### 1.定义
节点是ros2中独立可执行单元，本质是一个linux进程，每个节点只负责单一功能（模块化思想）
核心特性：
- 1.单功能原则：一个节点尽量只做一件事（高内聚，低偶合）
- 2.跨语言：c++/Python/java均可编写节点
- 3.分布式：可运行在同一主机，也可跨多台主机
- 4.生命周期：初始化->运行循环->销毁退出

