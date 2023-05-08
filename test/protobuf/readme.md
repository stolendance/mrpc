### 基本使用

protobuf 它能够将 一个对象 编码成二进制,作为传输的中间文件 ,并能够将中间文件解析回原数据创建.proto文件 ,定义好相应的数据类型

得到 xxx.proto文件

protoc test.proto --cpp_out="./"  // 生成相应的代码,包含数据类型的编码

// main.cpp 为调用proto的代码, 链接动态库进行编译

g++ test.pb.cc main.cpp -o test  `pkg-config --cflags --libs protobuf` -lpthread

#### 文件头

test.proto

```
// 指明版本及包名
syntax="proto3";// 声明proto版本

package fixbug;// 声明代码所在的包(对c++来说就是namespace)

```

#### 基本对象的定义

test.proto

string 定义成byte会更好

```

// 定义登陆请求消息类型
message LoginRequest
{
    string name=1; // 表明第一个字段
    string pwd=2; // 定义第二个字段
}
// 定义登陆响应消息类型
message LoginResponse
{
    int32 errcode=1;
    string errmsg=2;
    bool success=3;
}
```

#### 基本对象的初始化与赋值

main.cpp

```
  LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");
```

#### 基本对象的编码与解码

```
  
// 编码
string send_str;
    if(req.SerializeToString(&send_str))
    {
        cout<<send_str.c_str()<<endl;
    }
// 解码
LoginRequest reqB;
    if(reqB.ParseFromString(send_str))
    {
        cout<<reqB.name()<<endl;
        cout<<reqB.pwd()<<endl;
    }
```

#### 对象中的“组合" 定义

一个对象中包含另一个对象即为组合

test.proto

```
message ResultCode
{
    int32 errcode=1;
    string errmsg=2;
}

// 定义登陆响应消息类型
message LoginResponse
{
    ResultCode result=1;
    bool success=2;
}
```

#### 对象中的"组合" 初始化与赋值

main.cpp

```
LoginResponse rsp;
    ResultCode *rc=rsp.mutable_result();
    rc->set_errcode(1);
    rc->set_errmsg("登陆处理失败");
```

#### 列表定义

```
message User
{
    bytes name;
    uint32 age=2;
    enum Sex
    {
        MAN=0;
        WOMAN=1;
    }
    Sex sex=3;
}
message GetFriendListsResponse
{
    ResultCode result=1;
    repeated User users=2;
}
```

#### 列表的初始化

test.proto

```
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
```

#### 遍历列表

main.cpp

```
cout<<rlist.friend_lists_size()<<endl;
    for(auto item:rlist.friend_lists())
    {
        cout<<item.name()<<endl;
        cout<<item.age()<<endl;
    }

     return 0;
```

创建 XXX.proto 文件

```
// 指明版本及包名
syntax="proto3";// 声明proto版本

package fixbug;// 声明代码所在的包(对c++来说就是namespace)


// 基本对象的定义



```
