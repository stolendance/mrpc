#include"MrpcConfig.h"
#include<iostream>
#include<string>
#include<iostream>
#include<string>
void MrpcConfig::LoadConfigFile(const char* config_file)
{
    FILE *pf=fopen(config_file,"r");
    if(pf==nullptr)
    {
        std::cout<<config_file<< " is invalid"<<std::endl;
        exit(EXIT_FAILURE);
    }
    while(!feof(pf))
    {
        //三种情况 
        // 注释
        // 正确的配置项 =
        // 去掉开头多余的空格
        char buf[512]={0};
        fgets(buf,512,pf);
        std::string src_buf(buf);
        // 去掉字符串前面的空格
        src_buf=Trim(src_buf);
        // 判断#的注释
        if(src_buf[0]=='#'||src_buf.empty())
        {
            continue;
        }
        int idx=src_buf.find('=');
        if(idx==-1)
        {
            // 配置项不合法
            continue;
        }
        std::string key;
        std::string value;
        // 0123456  7-2
        key=src_buf.substr(0,idx);
        // 
        src_buf=src_buf.substr(idx+1,src_buf.size()-idx-1);

        m_configure[key]=src_buf;
    }

}
std::string MrpcConfig::Load(const std::string &key)
{
    if(m_configure.count(key))
    {
        return m_configure[key];
    }
    else
    {
        return "";
    }
}
std::string MrpcConfig::Trim(std::string src_buf)
{
    int idx=src_buf.find_first_not_of(' ');
    if(idx!=-1)
    {
        src_buf=src_buf.substr(idx,src_buf.size()-idx);
    }
    // 去掉字符串后面多余的空格
    idx=src_buf.find_last_not_of(' ');
    if(idx!=-1)
    {
        // 说明字符串后面有空格
        src_buf=src_buf.substr(0,idx);
    }
    return src_buf;
}
