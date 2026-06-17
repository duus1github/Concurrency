import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, TimerAction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    pkg_name = "learning_cpp"
    pkg_share = get_package_share_directory(pkg_name)
    urdf_path = os.path.join(pkg_share, "urdf", "gazebo_robot.urdf")
    use_sim_time = LaunchConfiguration("use_sim_time")

    arg_use_sim_time = DeclareLaunchArgument(
        "use_sim_time",
        default_value="true",
        description="启用仿真时间"
    )

    # 启动 Gazebo 服务端（加载两个插件）
    start_gzserver = ExecuteProcess(
        cmd=[
            'gzserver',
            '-s', 'libgazebo_ros.so',           # 发布 /clock 等
            '-s', 'libgazebo_ros_factory.so'    # 提供 spawn 服务
        ],
        output='screen',
        env=os.environ.copy()
    )

    # 启动 Gazebo 客户端
    start_gzclient = ExecuteProcess(
        cmd=['gzclient'],
        output='screen',
        env=os.environ.copy()
    )

    # 延时 2 秒后 spawn 机器人（直接加载文件，不依赖话题）
    spawn_robot = TimerAction(
        period=2.0,
        actions=[
            Node(
                package='gazebo_ros',
                executable='spawn_entity.py',
                arguments=[
                    '-file', urdf_path,
                    '-entity', 'my_robot',
                    '-x', '0.0', '-y', '0.0', '-z', '0.2'
                ],
                output='screen'
            )
        ]
    )

    # 延时 2.5 秒后启动 robot_state_publisher（发布 TF）
    robot_state_pub = TimerAction(
        period=2.5,
        actions=[
            Node(
                package='robot_state_publisher',
                executable='robot_state_publisher',
                parameters=[
                    {'robot_description': open(urdf_path).read()},
                    {'use_sim_time': use_sim_time}
                ],
                output='screen'
            )
        ]
    )

    return LaunchDescription([
        arg_use_sim_time,
        start_gzserver,
        start_gzclient,
        spawn_robot,
        robot_state_pub
    ])