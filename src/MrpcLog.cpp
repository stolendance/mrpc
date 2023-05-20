#include"MrpcLog.h"
#include<thread>
#include<iostream>
#include<stdio.h>
MrpcLog& MrpcLog::getInstance()
{
    static MrpcLog instance;
    return instance;
}
MrpcLog::MrpcLog()
{
    std::thread read_task([&](){
        for(;;)
        {
            std::string data=read_log();
            time_t t;
            time(&t);
            tm* tt=localtime(&t);
            char filename[100];
            sprintf(filename,"%d-%d-%d",tt->tm_year+1900,tt->tm_mon+1,tt->tm_mday);
            FILE* fp=fopen(filename,"a");
            char tmt[100];
            sprintf(tmt,"%d::%d::%d",tt->tm_hour+1,tt->tm_min+1,tt->tm_sec);
            fprintf(fp, "%s  %s\n",ctime(&t),data.c_str());
            fclose(fp);
        }
    });
    read_task.detach();
}
std::string MrpcLog::read_log()
{
    std::string data=line.pop();
    return data;
}

void MrpcLog::log(std::string data)
{
    line.push(data);
}