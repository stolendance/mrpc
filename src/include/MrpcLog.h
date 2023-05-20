#pragma once
#include<queue>
#include<iostream>
#include<string>
#include"Lockqueue.h"
class MrpcLog
{
    public:
        static MrpcLog & getInstance();
        std::string read_log();
        void log(std::string data);
    private:
        Lockqueue<std::string> line;
        MrpcLog();
        MrpcLog(const MrpcLog &&)=delete;
        MrpcLog(MrpcLog &)=delete;
        MrpcLog& operator=(const MrpcLog &)=delete;
};