#pragma once
#include "google/protobuf/service.h"
// 框架提供的专门服务发布rpc的网络对象类
#include<memory>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<map>
#include<string>
#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>

class RpcProvider
{
public:
    // 这里是框架提供的外部进行注册
    // void NotifyServise(UserService* service) 不能依赖于某个具体业务!
    void NotifyService(google::protobuf::Service * service);

    // 启动网络调用函数
    void Run();

    void OnConnection(const muduo::net::TcpConnectionPtr &conn);

    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);

private: 
    struct ServiceInfo
    {
        google::protobuf::Service* service_ptr;
        std::map<std::string,const google::protobuf::MethodDescriptor* > method_dic;
    };
    std::map<std::string,ServiceInfo> service_dic;
    // 组合了evenetloop
    muduo::net::EventLoop m_eventLoop;
};