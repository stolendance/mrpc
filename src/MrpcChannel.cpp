#include"MrpcChannel.h"
#include"rpc_header.pb.h"
#include<string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "MrpcApplication.h"
#include "Zookeeperutil.h" 
void MrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                        google::protobuf::Message* response, google::protobuf::Closure* done)
{
    //需要 得到 序列化后的字节流
    // 4字节代表rpc_header长度 rpc_header args
    //  先得到rpc_header
    src::RpcHeader header;
    // rpc_header 包含 service_name,method_name,args_len
    const google::protobuf::ServiceDescriptor*  service=method->service();
    std::string service_name=service->name();
    std::string method_name=method->name();
    //args序列化 并得到长度
    std::string args_str;
    request->SerializeToString(&args_str);
    uint32_t args_len=args_str.size();
    header.set_args_len(args_len);
    header.set_method_name(method_name);
    header.set_service_name(service_name);
    //rpc_header 序列化
    std::string rpc_header_str;
    header.SerializeToString(&rpc_header_str);
    std::string rpc_header_len_str;
    uint32_t rpc_header_len=rpc_header_str.size();
    rpc_header_len_str.insert(0,std::string((char*)&rpc_header_len,4));
    
    // 字符串都已经准备好
    std::string send_str;
    send_str+=rpc_header_len_str;
    send_str+=rpc_header_str;
    send_str+=args_str;

    std::string zoo_ip=MrpcApplication::getConfig().Load("zookeeperip");
    uint16_t zoo_port=stoi(MrpcApplication::getConfig().Load("zookeeperport"));

    
    std::string zoo_str="/"+service_name+"/"+method_name;
    Zookeeperutil zk=Zookeeperutil();
    zk.start();
    std::string ip_port=zk.getData(zoo_str);
    if(ip_port.size()==0)
    {
        std::cout<<"未找到zookeeper"<<std::endl;
        exit(EXIT_FAILURE);
    }
    int idx=ip_port.find_first_of(':');
    std::string ip=ip_port.substr(0,idx);
    uint16_t port=std::stoi(ip_port.substr(idx+1,ip_port.size()-1-idx));
    std::cout<<"得到rpc服务ip:"<<ip<<std::endl;
    std::cout<<"得到rpc服务端口:"<<port<<std::endl;
    // //  取得ip和地址
    // std::string ip=MrpcApplication::getConfig().Load("rpcserverip");
    // uint16_t port=stoi(MrpcApplication::getConfig().Load("rpcserverport"));
    // socket编程 发送信息
    int client_sock=socket(AF_INET,SOCK_STREAM,0);
    if(client_sock==-1)
    {
        controller->SetFailed("client_sock申请失败");
        return;
    }
    struct sockaddr_in sock_addr;
    sock_addr.sin_family=AF_INET;
    sock_addr.sin_port=htons(port);
    sock_addr.sin_addr.s_addr=inet_addr(ip.c_str());
    if(connect(client_sock,(sockaddr*)&sock_addr,sizeof(sock_addr))<0)
    {
        controller->SetFailed("连接失败");
        return;
    }
    // 发送信息
    int len=0;
    if(len=send(client_sock,send_str.c_str(),send_str.size(),0)<0)
    {
        controller->SetFailed("发送失败");
        return;
    }
    char buf[1024]={0};
    int len2=0;
    if((len2=recv(client_sock,buf,1024,0))<0)
    {
        controller->SetFailed("接受失败");
        return;
    }
    std::string response_str=buf;
    response->ParseFromArray(buf,len2);
    //std::cout<<"success: "<<std::endl;
    //response->ParseFromString(response_str);
}

