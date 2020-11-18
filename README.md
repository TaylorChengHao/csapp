<!--
 * @Author: your name
 * @Date: 2020-11-17 16:08:29
 * @LastEditTime: 2020-11-18 16:39:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \git_proj\csapp\tiny_web\READ.md
-->
# echo服务器
编译此例程：
```sh
make
```
服务器：
```sh
./server 10000      #使用一个未被占用的端口号
```
客户端：
```sh
./client x.x.x.x  10000     #ip和端口号
```
此echo服务器是一个在while循环中Accept连接和echo的业务逻辑的单线程服务器，而且是socket阻塞式的。在多个client连接同一个server时，除非头一个连接断开业务结束，不然后面的连接都无法建立。

# TINY Web服务器
根据csapp的11.6开发一个虽小但是功能齐全的TINYWeb服务器，结合了进程控制、UnixIO、套接字接口和HTTP。虽然他缺乏一个实际服务器所具有的功能性、健壮性和安全性，但是它足够用来为实际的浏览器提供静态的和动态的内容。
1.TINY 的main函数
这个TINY还是一个迭代服务器，和上面的echo服务器的基本架构是一样的。都是一个while循环中处理一个客户端。可以直接替换隔壁的echo服务器的echo函数，因为本质上Web服务就很类似echo，一次客户端请求对应一次服务端回应。
这个让我想到陈硕大佬在《Linux多线程服务端编程：使用muduo C++网络库》的附录A.1.11中说得：
TCP网络编程有是三个例子最值得学习研究，分别是echo、chat、proxy，都是长连接协议。
echo的作用：服务端被动接受新连接、收发数据、被动处理连接断开。每个连接之间是独立服务，连接之间没有关系。一个简单的HTTP的就是echo的变种。