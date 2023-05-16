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
    // 初始化相关参数
    
    fixbug::LoginRequest request;
    request.set_name("云霞川");
    request.set_pwd("hch12345");
    fixbug::LoginResponse response;
    stub.Login(nullptr,&request,&response,nullptr);
    // 会自动调rpcchannel  用callmethod

    // 得到执行后的response
    if(!response.success())
    {
        std::cout<<"远程rpc函数执行失败"<<std::endl;
        std::cout<<"errcode:"<<response.result().errcode()<<std::endl;
        std::cout<<"errmsg:"<<response.result().errmsg()<<std::endl;
        return 0;
    }
    std::cout<<"远程rpc函数执行成功"<<std::endl;
    std::cout<<"success: "<<response.success()<<std::endl;

    fixbug::HelloRequest request2;
    request2.set_name("云霞川");
    fixbug::HelloReponse response2;
    stub.Hello(nullptr,&request2,&response2,nullptr);
    // 会自动调rpcchannel  用callmethod

    // 得到执行后的response
    std::cout<<response2.hello()<<std::endl;
    std::cout<<"远程rpc函数执行成功"<<std::endl;
    


}