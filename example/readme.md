# example

表明如何调用框架

1. 如何将本地函数注册为rpc函数
2. 远程如何调用

# callee

## 本地函数注册为rpc函数(业务代码)

在example 中提供

展示了callee 如何将本地函数注册为rpc函数

#### userServiceRpc和userServiceRpc_stub的本质

使用框架的时候要明确一件事

1. 框架让我们实现什么
   ee端继承。userServiceRpc , 我们需要继承并用本地函数覆盖这两个函数
   这样就相当于本地函数注册了

   ![1683533468488](image/readme/1683533468488.png)

在这个函数中,我们会得到 request 输入参数。response 返回的空对象。done 回调函数

我们需要根据函数逻辑处理参数, 并返回结果赋值给response

## 框架的设计

### 框架的使用

声明一个对象继承xxxServiceRpc,重写其中注册的方法,使用本地方法进行覆盖

调用的时候,先初始化一个rpc对象(单例模式全局可用),

声明一个网络服务对象

声明本地函数服务对象,注册到网络服务对象中, 然后启动

(很简单的使用,理想化的设计)

example/callee/ userservice.cpp

```
#include<iostream>
#include<string>
#include"user.pb.h"
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
    provider.NotifyService(new ProductService());

    //启动rpc
    provider.run()

    return 0;
}
```
