#include<zookeeper/zookeeper.h>
#include"Zookeeperutil.h"
#include"MrpcApplication.h"
#include<semaphore.h>
#include<iostream>
// 全局的watcher观察器, zkserver给zkclient的通知
void global_watcher(zhandle_t* zh,int type,int state,const char *path, void* wwatcherCtx)
{
    if(type==ZOO_SESSION_EVENT)
    {
        if(state==ZOO_CONNECTED_STATE)
        {
    
            sem_t* sem=(sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}
Zookeeperutil::Zookeeperutil()
{
    ;
}
void Zookeeperutil::start()
{
    std::string host=MrpcApplication::GetInstance().getConfig().Load("zookeeperip");
    std::string port=MrpcApplication::GetInstance().getConfig().Load("zookeeperport");
    std::string connstr=host+":"+port;

    /*
	zookeeper_mt：多线程版本
	zookeeper的API客户端程序提供了三个线程
	API调用线程 
	网络I/O线程  pthread_create  poll
	watcher回调线程 pthread_create
	*/
    m_handle=zookeeper_init(connstr.c_str(),global_watcher, 30000, nullptr, nullptr, 0);
    if(m_handle==nullptr)
    {
        std::cout<<"zookeeper_init error"<<std::endl;
        exit(EXIT_FAILURE);
    }
    sem_t sem;
    sem_init(&sem,0,0);
    zoo_set_context(m_handle,&sem);
    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}
void Zookeeperutil::create(std::string path, std::string data, int state)
{
    // 判断path是否存在,存在就不需要创建了
    // service节点下:方法名节点(存取数据ip:port)
    char path_buffer[128];
    int bufferlen=sizeof(path_buffer);
    int flag=zoo_exists(m_handle, path.c_str(),0, nullptr);
    if(flag==ZNONODE)
    {
        //该节点不存在
        flag=zoo_create(m_handle,path.c_str(),data.c_str(),data.size(),
        &ZOO_OPEN_ACL_UNSAFE,state,path_buffer,bufferlen);
        if(flag==ZOK)
        {
            std::cout<<"znode create success"<<std::endl;
        }
        else
        {
            std::cout<<"flag::"<<flag<<std::endl;
            std::cout<<"znode create failure"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string Zookeeperutil::getData(std::string path)
{
    char buffer[64];
    int bufferlen=sizeof(buffer);
    int flag=zoo_get(m_handle,path.c_str(),0,buffer,&bufferlen,nullptr);
    if(flag!=ZOK)
    {
        std::cout<<"get znode error"<<std::endl;
        return "";
    }
    else
    {
        return std::string(buffer);
    }
}

Zookeeperutil::~Zookeeperutil()
{
    if (m_handle != nullptr)
    {
        zookeeper_close(m_handle); // 关闭句柄，释放资源  MySQL_Conn
    }
}