#pragma once
#include "google/protobuf/service.h"
// 框架提供的专门服务发布rpc的网络对象类
class RpcProvider
{
public:
    // 这里是框架提供的外部进行注册
    // void NotifyServise(UserService* service) 不能依赖于某个具体业务!
    void NotifyService(google::protobuf::Service * service);

    // 启动网络调用函数
    void Run();
};