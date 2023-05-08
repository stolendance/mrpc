#include<iostream>
#include<string>
#include "test.pb.h"
using namespace fixbug;
using namespace std;
int main()
{

    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化-》char*
    string send_str;
    if(req.SerializeToString(&send_str))
    {
        cout<<send_str.c_str()<<endl;
    }

    // 从send_str 反序列化一个login请求对象

    LoginRequest reqB;
    if(reqB.ParseFromString(send_str))
    {
        cout<<reqB.name()<<endl;
        cout<<reqB.pwd()<<endl;
    }

    LoginResponse rsp;
    ResultCode *rc=rsp.mutable_result();
    rc->set_errcode(1);
    rc->set_errmsg("登陆处理失败");

    GetFriendListsResponse rlist;
    ResultCode* rc2=rlist.mutable_result();
    rc2->set_errcode(0);

    User *user1=rlist.add_friend_lists();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User *user2=rlist.add_friend_lists();
    user2->set_name("zhang san");
    user2->set_age(20);
    user2->set_sex(User::MAN);

    cout<<rlist.friend_lists_size()<<endl;
    for(auto item:rlist.friend_lists())
    {
        cout<<item.name()<<endl;
        cout<<item.age()<<endl;
    }

     return 0;





}