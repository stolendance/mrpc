#include<iostream>
#include<string>
#include"user.pb.h"
#include"MrpcApplication.h"
#include"RpcProvider.h"
/*
#include "../user.pb.h"  不需要 因为外层的cmakelist已经设定好搜寻头文件的地址
userservice 原本是一个本地服务,提供了两个进程内的本地方法,login和getfriendlists
*/
// 
class UserService:public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout<<"doing local service: Login"<<std::endl;
        std::cout<<"name: "<<name<<"pwd:"<<pwd<<std::endl;
        return true;
    }

    /*
    重写基类UserServiceRpc的虚函数,下面这些方法都是框架直接调用的

    caller->login(loginrequest)->
    
    */
    void Login(::google::protobuf::RpcController* controller,
                const ::fixbug::LoginRequest* request,
                ::fixbug::LoginResponse* response,
                ::google::protobuf::Closure* done)
    {
        // 框架给业务上爆了请求参数LoginRequest,业务获取相应数据做本地业务
        std::string name=request->name();
        std::string pwd=request->pwd();

        std::cout<<"rpc_login name: "<<name<<std::endl;
        std::cout<<"rpc_login pwd: "<<pwd<<std::endl;
        // 做本地业务
        bool login_result=Login(name,pwd);
        // 把响应写入
        fixbug::ResultCode * code=response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);
        // 执行回调操作 执行响应对象数据的序列化和网络发送（都是由框架来完成的)
        done->Run();

    }


};
int main(int argc,char **argv)
{
    // 调用框架的初始化操作
    MrpcApplication::Init(argc,argv);

    // 把UserService对象发布到rpc节点上
    RpcProvider provider; // 网络发布对象 
    provider.NotifyService(new UserService());

    //启动rpc
    provider.Run();

    return 0;
}