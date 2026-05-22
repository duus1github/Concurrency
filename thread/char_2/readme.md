第二章 线程管理
=========
本章核心:学会操控线程本身
==========
1.启动线程
- std::thread t(函数名,参数,...)
- 可传:普通函数,lambda,成员函数,函数对象

2.等待线程结束join()
- 调用后主线程阻塞,直到子线程跑完
- 必须join,否则程序退出会漰溃

3.分离线程detach()
- 线程后台运行,不受主线程控制
- 严禁访问主线程局部变量

4.参数传递
- 默认拷贝到线程内部
- 传引用必须用std:ref
- 只移动对象用std::move

5.线程所有权转移
- thread只能移动(move),不能拷贝
- 可以放进vector<thread>批量管理

6.RALL安全线程
- 确保异常时也能自动join
- 书本例子:thread_guard / scoped_thread
- explicit 禁止隐式转换

7.获取cpu核心数
- std::thread::hardware_concurrency()