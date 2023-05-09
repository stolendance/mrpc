#pragma once
#include"MrpcConfig.h"
// mrpc 框架的初始化类
// 框架只需要一个 因此用单例模式进行设计

class MrpcApplication
{
public:
    static void Init(int argc, char ** argv);
    static MrpcApplication& GetInstance();
private:
    static MrpcConfig m_config;
    MrpcApplication(){};
    MrpcApplication(const MrpcApplication & other)=delete;
    MrpcApplication(MrpcApplication && other)=delete;
    MrpcApplication& operator =(const MrpcApplication& other)=delete;
    ~MrpcApplication(){};
};