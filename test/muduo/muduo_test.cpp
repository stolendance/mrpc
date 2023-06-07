#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<string>
#include<iostream>
#include<functional>
void OnConnection(const muduo::net::TcpConnectionPtr &conn)
{

}
void OnMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer* buffer,muduo::Timestamp timestamp)
{
    conn->send("received\n");
    conn->send(buffer);
}
int main()
{
    // 绑定地址及端口
    std::string ip="127.0.0.1";
    uint16_t port=8032;
    muduo::net::InetAddress address(ip,port);
    // 创建event_loop 相当于 reactor模型中的事件分发器
    muduo::net::EventLoop event_loop;
    // 创建服务器,相当于reactor
    muduo::net::TcpServer server(&event_loop,address,"reactor");
    server.setThreadNum(4);
    // 绑定相关函数
    server.setConnectionCallback(std::bind(&OnConnection,std::placeholders::_1));
    server.setMessageCallback(std::bind(&OnMessage,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    server.start();
    event_loop.loop();
}