#include<string>
#include<zookeeper/zookeeper.h>
class Zookeeperutil
{
public:
    Zookeeperutil();
    ~Zookeeperutil();
    void start();
    void create(std::string path, std::string data, int state);
    std::string getData(std::string path);
private:
    zhandle_t *m_handle;
};