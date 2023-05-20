#include<zookeeper/zookeeper.h>
#include"Zookeeperutil.h"
#include"MrpcApplication.h"
Zookeeperutil::Zookeeperutil()
{
    ;
}
void Zookeeperutil::start()
{
    std::string host=MrpcApplication::GetInstance().getConfig().Load("zookeeperip");
    std::string port=MrpcApplication::GetInstance().getConfig().Load("zookeeperport");
    std::string connstr=host+":"+port;
}