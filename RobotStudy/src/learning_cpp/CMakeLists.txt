cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0148 OLD)
project(learning_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp_action REQUIRED)
# 1. 先生成自定义服务接口
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/AddTwoInts.srv"
  action/Progress.action
  DEPENDENCIES builtin_interfaces
)

# 公共依赖列表
set(COMMON_DEPS rclcpp std_msgs rclcpp_action)

# ==================== 普通节点(不使用自定义srv) ====================
add_executable(hello_cpp src/node_hello.cpp)
ament_target_dependencies(hello_cpp ${COMMON_DEPS})

add_executable(talker src/talker.cpp)
ament_target_dependencies(talker ${COMMON_DEPS})

add_executable(listener src/listener.cpp)
ament_target_dependencies(listener ${COMMON_DEPS})

# ==================== 使用本包自定义 srv 的节点(核心修复) ====================
# 获取 C++ 类型支持目标
rosidl_get_typesupport_target(cpp_ts_target ${PROJECT_NAME} "rosidl_typesupport_cpp")

# 服务端
add_executable(service_server src/service_server.cpp)
ament_target_dependencies(service_server ${COMMON_DEPS})
# 链接接口类型支持
target_link_libraries(service_server "${cpp_ts_target}")

# 客户端
add_executable(service_client src/service_client.cpp)
ament_target_dependencies(service_client ${COMMON_DEPS})
target_link_libraries(service_client "${cpp_ts_target}")
# action
add_executable(action_server src/action_server.cpp)
ament_target_dependencies(action_server ${COMMON_DEPS})
target_link_libraries(action_server "${cpp_ts_target}")

add_executable(action_client src/action_client.cpp)
ament_target_dependencies(action_client ${COMMON_DEPS})
target_link_libraries(action_client "${cpp_ts_target}")

# 参数
add_executable(param_node src/param_node.cpp)
ament_target_dependencies(param_node rclcpp)
target_link_libraries(param_node  "${cpp_ts_target}")

# logdemo
add_executable(log_demo_node src/log_demo.cpp)
ament_target_dependencies(log_demo_node rclcpp)
target_link_libraries(log_demo_node  "${cpp_ts_target}")

# 单节点综合功能
add_executable(composite_node src/composite_node.cpp)
ament_target_dependencies(composite_node ${COMMON_DEPS})
target_link_libraries(composite_node  "${cpp_ts_target}")
# 安装
install(TARGETS
  hello_cpp
  talker
  listener
  service_server
  service_client
  action_server
  action_client
  param_node
  log_demo_node
  composite_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()