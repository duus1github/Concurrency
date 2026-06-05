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


