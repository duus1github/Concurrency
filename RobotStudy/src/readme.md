# 第三章 服务通信
## 3.1 概念
#### 1.服务通信模型：
- 角色：服务端service + 客户端 client
- 流程：客户端发起请求，服务端接受并处理，服务端返回应答，客户端接收结果（一问一答，同步阻塞）
- 特点：客户端发送请求后，会暂停执行，直到收到服务端返回结果才继续运行

## 3.2 三大通信核心区别
### topic:
- 通信模式：异步，发布/订阅，多对多
- 特点：无应答，持续数据流
- 场景：传感器，地盘速度，图像，IMU
### service:
- 通信模式：同步，请求/响应，一对一
- 特点：一问一答，阻塞等待
- 场景：启停，参数查询，单次指令
### action
- 通信模式：异步目标+实时反馈+结果
- 特点：长任务，可取消，持续回传进度
- 场景：导航，机械臂运动，延时作业，巡航

## ros日志系统
### 基础说明
ros2 内置分级日志，自带时间戳，节点名，日志级别，支持终端筛选，文件落盘，是项目标准输出方式
#### 1.日志级别：
- 1.debug:调试信息，开发排查问题用，默认不输出
- 2.普通运行信息(最常用)
- 3.warn:警告，非致命异常
- 4.error:错误，功能异常，但节点不退出
- 5.fatal:致命错误，节点直接终止
#### 2.开启DEBUG日志
- 1.启动节点临时设置日志级别
ros2 run learning_cpp log_demo_node --ros-args --log-level DEBUG
- 2.代码内设置
this->get_logger().set_level(rclcpp::LoggingLevel::Debug);

## 单节点融合

## urdf模型
### 启动指令为：
   ros2 launch urdf_tutorial display.launch.py model:=$(realpath src/learning_cpp/urdf/robot_with_joint.urdf)

## DDS
### 定义：
全称：Data Distribution Service,也就是数据分发服务，是一套专门为实时系统设计的数据分发/订阅标准，记住是一种通信标准

## 第六章 TF2坐标变换
### 1.定义
TF2 是ros2官方提供的坐标变换功能包，专门用了维护机器人上多个坐标系之间的相对位置与姿态关系。
机器人的每个部件都有独立的坐标系，TF2会实时记录、转发这些坐标系的变换数据，实现跨坐标系数据换算。
#### 1.2 核心术语
##### 坐标系
- 机器人根坐标系：base_link(底座，所有坐标系的根节点)
- 传感器坐标系： laser_link(雷达)，camera_link(相机)
- 运动关节坐标系：pan_link(云台)，wheel_link(轮子)
##### 坐标变换
描述子坐标系相对于父坐标系的空间关系，由两部分组成：
- 平移：x/y/z 三轴偏移
- 旋转：姿态，支持欧拉角（RPY）或四元素
- Transform 变换：子坐标系相对于父坐标系的空间偏移
- TF树：所有坐标系按父子关系组成的树型拓扑，可视化查看
##### 核心消息类型
geometry_msgs/msgs/TransformStamped
字段：时间戳，父frame,子frame,translation,rotation四元素旋转

#### 1.3 两种TF变换
- 1.静态TF/tf_static
部件位置永久固定，雷达，外置相机，固定支架，只发布一次，永久生效
- 2.动态TF/tf
部件会运动，旋转平台，车轮，机械臂，持续高频刷新位姿

## 第七章 常用传感器与数据处理
### 7.1 ros2 标准传感器消息总览
#### 1.定义
ros2 内置sensor_msgs功能包，封装机器人各类传感器统一标准消息结构，所有硬件，仿真输出数据都遵循该格式，实现跨设备通用解偶，无需自定义消息
#### 2.原理
1.硬件驱动，仿真节点采集传感器原始数据，封装进sensor_msgs标准消息
2.通过话题发布，算法节点统一订阅解析
3.消息内置时间戳，坐标系标识heard.frame_id,天然适配TF坐标变换
#### 3.应用场景
- 激光雷达避障，SLAM建图
- IMU姿态解算，里程计融合
- 深度相机，RGB图像视觉识别
- 超声波，红外近距离探测。
#### 4.核心常用消息清单
sensor_msgs/msg/LaserScan   单线激光传感器
sensor_msgs/msg/Imu         IMU惯性单元
sensor_msgs/msg/Image       相机RGB图像
sensor_msgs/msg/PointCloud2 3D雷达/深度相机点云
sensor_msgs/msg/Range       超声波/红外测距

### 7.2 单线激光雷达LasterScan
#### 1.定义
sensor_msgs/msg/LaserScan 是单线2D激光雷达专用标准消息，存储一整圈扫描测距数据，雷达硬件参数，是移动机器人避障，slam最基础数据载体。
#### 2.参数
std_msgs/Header header
    uint32 seq       # 帧序号
    builtin_interfaces/Time stamp    #时间戳
    string           # 雷达坐标系（laser_link,TF匹配关键）
float32 angle_min    # 激光扫描起始角度(弧度)
float32 angle_max    # 扫描结束角度(弧度)
float32 angle_increment # 单次测距角度步长
float32 time_increment  # 两次测距时间间隔
float32 scan_time    # 完整一圈扫描耗时
float32 range_min    # 雷达最小一圈探测距离
float32 range_max    # 雷达最大探测距离
float32[] ranges     # 核心：每个角度对应的障碍物距离数组
float32[] intensities # 反射强度(可选)
#### 工作逻辑
雷达旋转测距，按角度顺序存入ranges数组，通过frame_id绑定雷达TF坐标系，可直接结合TF转换到地盘base_link.
#### 3.应用场景
1.自主移动机器人实时避障
2.2D姗格地图slam构建
3.跟随，路径规划障碍物检测
4.內框，墙体环境轮廓识别。

### 7.3 IMU惯性传感器(sensor_msgs/msg/Imu)
#### 1.定义
sensor_msgs/msg/Imu 是ros2 统一惯性测量单元标准信息，用于承载六轴/九轴IMU原始解算数据，包含三轴加速度，三轴角速度，四元数姿态，自带坐标系标识与误差协方差，所有实车，仿真IMU设备统一输出该格式。
#### 2.数据结构
"""
std_msgs/Header header
    builtin_interfaces/Time stamp               # 数据时间戳，用于TF时序匹配
    string frame_id                             # IMU坐标系 imu_link 绑定tf变换
geometry_msgs/msg/Quaternion orientation        # 姿态四元素(机体相对世界)
double[9] orientation_covariance                # 姿态误差协方差

geometry_msgs/msg/Vector3 angular_velocity      # 三轴角速度 rad/s
double[9] angular_velocity_covariance           # 角速度误差

geometry_msgs/msg/Vector3 linear_acceleration   # 三轴加速度 m/s^2
double[9] linear_acceleration_covariance        # 加速度误差
"""
1.物理逻辑：加速度计感知机身线性加速度;陀螺仪感知旋转角速度;内部滤波融合输出稳定四元素姿态
2.坐标系逻辑：所有加速度，角速度，姿态均基于imu_link局部坐标系，需要TF转换到地盘base_link统一基准
3.协方差矩阵：描述传感器噪声大小，用于EKF,UKF融合算法加权滤波

#### 3.应用场景
1.移动机器人姿态解算：获得机身俯仰，横滚，偏航角，判断车身是否倾斜。
2.里程计融合导航：补偿轮式里程计打滑，漂移误差。
3.平衡机器人，两轮自平衡车核心反馈数据
4.云台防抖，视觉图像防抖
5.SLAM紧偶合，优化地图定位精度

### 7.4 三维点云sensor_msgs/msg/point_cloud2
#### 1.定义
sensor_msgs/msg/PointCloud2是ROS2标准三维点云消息，专门存储深度相机，3d激光雷达输出的海量三维空间点集合
每个点保护三维坐标x/y/z,可拓展携带反射强度，RGB色彩，法向量等数据，消息头部自带坐标系标识，可配合TF完成全场景坐标统一换算，是机器人三维感知通用标准数据格式
#### 2.数据结构
std_msgs/Header header        #标准消息头
  builtin_interfaces/msg/Time stamp # 采集时间戳，用于TF时序同步
  string frame_id                   # 点云所属局部坐标系
uint32 height                       # 点云行数；2D激光固定height=1,深度相机为多行
uint32 width                        # 点云列数
sensor_msgs/msg/PointField[] fields # 定义每个点包含哪些数据(x/y/z/intensity等)
uint32 point_step                   # 单个点所占字节大小
uint32 row_step                     # 一整行所有点占用字节大小
uint8[] data                        # 二进制点云缓冲区
bool is_bigendian                   # 数据字节序
bool is_dense                       # 是否不存在无效空点

#### 3.数据流转逻辑
1. 3D雷达/深度相机硬件采集空间节点，二进制打包存入data数组
2. fields字段标记每个偏移字节对应的数据含义，程序按偏移解析x/y/z
3. frame_id绑定传感器局部坐标系，通过TF矩阵运算可把全部三维点转换到底盘base_link统一基准
4. 区分：LaserScan仅2D平面测距，PointCloud2是完整三维空间数据

#### 4.应用场景
1.自动驾驶3D激光雷达：检测车辆，行人，路沿，障碍物
2.移动机器人三维避障：识别台阶，悬空障碍物，低矮物体
3.深度相机视觉：三维重建，物体尺寸测量
4.机械臂手眼抓取：获取目标物体三维世界坐标
5.高精度三维SLAM地图构建

## 第8章 Gazebo 仿真基础
### 1.Gazebo 定义
Gazebo是ros官方配套的三维物理仿真器，可在计算机内虚拟搭建机器人，地形，障碍物，各类传感器；依托物理引擎模拟重力，碰撞，惯性，摩擦力，仿真输出与真实硬件完全一致的ros标准话题，实现无实体硬件调试机器人算法。
### 2.原理
1. 物理引擎：默认ODE引擎，读取URDF中<collision>碰撞，<intertial>惯性参数，计算机器人动力学运动；
2. 通信桥接包ros_gz:打通Gazebo仿真世界与ros2节点，双向转发话题，服务
3. 插件机制：Gazebo提供雷达，IMU,相机，电机控制器插件，挂载到urdf连杆后自动仿真传感器数据；
4. 资源文件分类:
- .urdf:机器人本体模型
- .world:仿真环境(地面，墙体，障碍物，光照)
- launch文件：一建启动仿真世界，加载机器人，启动ros2节点
### 3.应用场景
1、前期开发：无实物机器人时，调试避障，导航，云台控制逻辑
2、批量测试：快速搭建多种复杂场景，避免实体设备反复运行造成磨损
3、教学学习：零硬件成本练习urdf，tf,传感器数据解析
4、参数标定：调试电机PID，运动控制参数，直观观察动力学效果
5、多机协同仿真，室内/厂区复杂环境复刻

### 8.3 Launch文件：一键启动Gazebo仿真
#### 1、定义
ros3 python launch脚步，批量完成三件事：启动gazebo仿真世界，加载机器人urdf模型，启动状态发布器解析TF
#### 2、原理
1.IncludeLaunchDescription调用官方Gazebo启动脚本，打开仿真窗口
2.Node启动robot_state_publisher加载URDF,发布TF，
3.全局参数use_sim_time:=true,强制所有节点使用仿真时钟(仿真必备)
#### 3、应用场景
日常仿真开发统一使用launch一键拉起环境，无需分多个终端手动启动仿真、模型、rviz
