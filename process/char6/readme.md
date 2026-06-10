# 第六章 高级I/O:多路复用(select/poll/epoll)
## 6.1 为什么需要多路复用
### 1.定义
I/O多路复用：一个进程同时监听多个文件描述符(socket)，哪个就绪就处理哪个，不用阻塞在单个连接上
### 2.原理
- 进程调用select/poll/epoll，把要监听的fd告诉内核
- 内核监视所有fd
- 任意一个fd就绪(有数据/可连接)，内核通知进程
- 进程只处理就绪的fd,效率极高
### 3.作用/场景
- 高并发服务器（1个进程处理成千上万个客户端）
- 替代多进程，多线程，减少资源消耗
- 后端服务，网关，中间件必备

## 6.2 select多路复用
### 1.定义
select是unix通用的多路复用函数，能同时监听多个fd的读、写、异常事件
### 2.原理
1、传入3个fd集合(读，写，异常)
2、内核轮询所有fd
3、将就绪的fd返回
4、进程遍历找到就绪fd并处理
### 3.作用/场景
- 跨平台兼容
- 简单并发场景
- 缺点：监听上限默认1024，效率低
### 4.api
int select(int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);

## 6.3 epoll多路复用(linux高性能王者)
### 1.定义
epoll是linux特有的高性能多路复用，解决select缺点，百万并发首选
### 2.原理
1.epoll_create() 创建epoll实列
2.epoll_ctl() 注册/删除fd(内核事件表)
3.epoll_wait() 阻塞等待就绪事件(只返回就绪，不用遍历)
4.效率O(1),高并发神器
### 3.作用/场景
- nginx,redis,网关，高并发服务器
- 生产环境100%用epoll
### 4.核心API
int epoll_create(int size)
int epoll_ctl(int epfd,int op,int fd,struct epoll_event *event)
int epoll_wait(int epfd,struct epoll_event *events,int max,int timeout)

# 第六章 socket 套接字
## 6.1 引言
套接字（socket）是unix/linux系统中通用的网络编程接口，也是实现不同主机、不同进程网络通信的基础。
### 1.定义
Socket是操作系统提供的网络通信端点，可以理解为·「网络版文件描述符」，应用程序通过Socket收发网络数据
### 2.原理
应用程序通过Socket调用内核网络协议栈，依托tcp/ip等协议完成跨主机数据传输
### 3.作用/场景
所有网络客户端，服务器开发的基础接口

## 6.2 概述
### 1.历史
套接字起源于BSD4.2版本unix,后被各大类unix,linux,windows等广泛兼容，成为行业标准接口，统称为套接字
### 2.功能
Socket屏蔽了底层硬件，协议差异，提供统一的编程接口
1.建立主机间通信连接
2.发送/接收网络数据
3.断开通信链接
4.配置网络参数，获取对方主机信息
### 3.套接字的三种类型
- 1.SOCK_STREAM:流式套接字，
    对应TCP协议
    面向连接，可靠，无报文边界，字节流传输
- 2.SOCK_DGRAM:数据报套接字
    对应UDP协议
    无链接、不可靠、保留数据边界
- 3.SOCK_RAW:原始套接字
    直接操作网络层（IP层），可收发底层数据包
    多用于协议测试，抓包，自定义协议
流式=TCP,数据报=Udp

## 6.3 linux支配的网络协议
linux系统默认以TCP/IP协议簇为核心，完全兼容标准套接字接口
Socket所有操作最终都会交由内核Tcp/ip协议

## 6.4 套接字地址
网络通信必须指定 ·「ip地址+断开」，linux用两套地址结构体，通用地址，IPV4专用地址
### 1.定义
Socket通信端点，由·「ip地址+端口号」唯一标识，一对Socket完成两端通信
### 2.Socket描述符
Socket创建后返回普通文件描述，用法和open返回的fd一致，可使用read/write读写数据
### 3.数据传输逻辑
数据从应用层->Socket层->内核协议层->网络链路，对方主机反向解析

## 6.10 五种I/O模式
### 1.阻塞I/O(默认模式)
- 原理：recv/accept无数据时，进程休眠阻塞
- 特点：简单，单链接效率低
### 2.非阻塞I/O
通过fcntl()设置Socket非阻塞，无数据直接返回，不等待
### 3.多路复用I/O
- select,一个进程监听多个fd
- 作用：单进程管理多客户端，高并发基础
### 4.信号驱动I/O
内核有数据时发送信号，通知进程，异步模型
### 5.异步I/O
内核全权完成读写，完成后通知应用，linux支持有限
### 对比
阻塞最简单，多路复用工程最常用，异步性能最高但兼容性差。
