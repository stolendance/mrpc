#include "RpcProvider.h"
#include "MrpcApplication.h"
#include "rpc_header.pb.h" 
#include<iostream>
#include<functional>
void RpcProvider::NotifyService(google::protobuf::Service * service){
    const google::protobuf::ServiceDescriptor* service_ptr =service->GetDescriptor();
    // 得到method的数量
    const std::string service_name=service_ptr->name();
    int n= service_ptr->method_count();
    std::map<std::string,const google::protobuf::MethodDescriptor* > method_dic;
    // 遍历方法 ,进行存储
    for(int i=0;i<n;i++)
    {
        const google::protobuf::MethodDescriptor * method_ptr=service_ptr->method(i);
        const std::string method_name=method_ptr->name();
        method_dic[method_name]=method_ptr;
    }
    ServiceInfo sinfo;
    sinfo.service_ptr=service;
    sinfo.method_dic=method_dic;
    service_dic[service_name]=sinfo;

    // for(auto item:service_dic)
    // {
    //     std::cout<<"服务名:"<<item.first<<std::endl;
    //     for(auto item2:item.second.method_dic)
    //     {
    //         std::cout<<"方法名:"<<item2.first<<std::endl;
    //     }·
    // }
}

// 启动rpc服务节点,开始提供rpc远程网络调用服务
void RpcProvider::Run(){

    // 下面这句代码是错的,因为是private 只能调用其中函数,不能把它进行复制
    //MrpcApplication instance = MrpcApplication::GetInstance();
    // cout<<"尝试改变"<<endl;
    MrpcConfig config_ins=MrpcApplication::GetInstance().getConfig();
    // config_ins["abc"]="def";

    std::string ip=config_ins.Load("rpcserverip");
    uint16_t port=std::stoi(config_ins.Load("rpcserverport"));
    // 需要地址及端口
    muduo::net::InetAddress address(ip,port);
     // 创建tcpserver对象
    muduo::net::TcpServer server(&m_eventLoop,address,"RpcProvider");
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    // 绑定链接回调和消息读写回调方法 很好地分离了网络代码和业务代码
    //server.setConnectionCallback();
    // 设置muduo库的线程数量
    server.setThreadNum(4);

    std::cout<<"Rpc start"<<" ip:"<<ip<<" port:"<<port<<std::endl;

    //启动网络服务
    server.start();
    m_eventLoop.loop();

    //std::cout<<"hello world"<<std::endl;
    

}
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{

}

// 当有字节流时, 会自动调用该函数
// 该函数 解析字符流 并调用相应注册的函数
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer* buffer,muduo::Timestamp timestamp)
{
    // 先转化为字符串
    std::string buffer_str=buffer->retrieveAllAsString();
    // 截取前四个字节
    uint32_t len=0;
    buffer_str.copy((char*)len,4,0);
    // 解析rpc_header对象
    std::string rpc_header_str=buffer_str.substr(4,len);
    src::RpcHeader rpcheader=src::RpcHeader();
    if(!rpcheader.ParseFromString(rpc_header_str))
    {
        // 解析失败
        std::cout<<"rpcheader解析失败"<<std::endl;
        return;
    }
    std::string service_name=rpcheader.service_name();
    std::string method_name=rpcheader.method_name();
    uint32_t args_len=rpcheader.args_len();
    // 找到相应的service method
    if(!service_dic.count(service_name))
    {
        std::cout<<"服务名不存在"<<std::endl;
        return ;
    }
    ServiceInfo service_info=service_dic[service_name];
    google::protobuf::Service* service=service_info.service_ptr;
    if(!service_info.method_dic.count(method_name))
    {
        std::cout<<"方法名不存在"<<std::endl;
        return;
    }
    const google::protobuf::MethodDescriptor* method_des=service_info.method_dic[method_name];

    // 截取 args 此时的args为request
    std::string args_str=buffer_str.substr(4+len,args_len);
    // 解析args ,需要获得 service method 下的request
    // 直接使用基类 message即可  message是一个抽象类 不能实例化
    // 因此还需要找method的方法  创建一个新的对象 
    google::protobuf::Message* request=service->GetRequestPrototype(method_des).New();
    google::protobuf::Message* response=service->GetResponsePrototype(method_des).New();
    if(!request->ParseFromString(args_str))
    {
        std::cout<<"解析参数失败"<<std::endl;
        return ;
    }
    // 调用该函数
    //service->CallMethod(method_des,nullptr,request,response,callback);

}