#include<iostream>
#include"MrpcApplication.h"
#include"MrpcChannel.h"
#include"user.pb.h"
int main(int argc, char** argv)
{
    // 要是想调用的话, 需要传入相关参数 ,进行配置,全局只会初始化一次
    MrpcApplication::Init(argc,argv);

    // 
    fixbug::UserServiceRpc_Stub stub(new MrpcChannel());
    // 定义好request和
    //stub.Login(....)
    // 会自动调rpcchannel  用callmethod



}