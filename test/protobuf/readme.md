### 基本使用

### proto库

#### protobuf 它能够将 一个对象 编码成二进制,作为传输的中间文件 ,并能够将中间文件解析回原数据创建.proto文件 ,定义好相应的数据类型

编写 xxx.proto文件

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

创建 XXX.proto 文件// 指明版本及包名

### rpc service

在前面protobuf中 定义了对象

接下来要在protobuf 中,定义相关的函数

#### 函数的注册

.proto

```
// 定义下列选项, 表明rpc服务定义方法需要生成
option cc_generic_services=true;

// 在proto中定义rpc方法类型  必须定义option 才能生成该代码
service UserServiceRpc 
{
    rpc Login(LoginRequest) returns(LoginResponse);
    rpc GetFriendLists(GetFriendListsRequest) 	returns(GetFriendListsResponse); 
}
```

#### 底层

#### message

```
定义一个数据对象
继承 google::protobuf::Service
提供
	setXXX() //数据初始化方法
	XXX() // 数据访问方法
	编码方法
	解码方法
```

#### service

#### callee ServiceProvider rpc 提供者 继承至google service

UserServiceRpc

具有默认构造函数,不需要传入任何参数

包含两个函数 及相应的参数  输入的参数作为一个const  输出的参数 作为一个空变量提供进去

```
class UserServiceRpc : public ::PROTOBUF_NAMESPACE_ID::Service {
 

  static const ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor* descriptor();

  virtual void Login(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done);
  virtual void GetFriendLists(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::fixbug::GetFriendListsRequest* request,
                       ::fixbug::GetFriendListsResponse* response,
                       ::google::protobuf::Closure* done);
```

#### caller ServiceConsumer rpc 服务消费者。继承至 UserServiceRpc( 提供者类)

#### UserServiceRpc_Stub 代理类

本地调用-> stub -> 网络 -> stub -> 服务器执行函数

无默认构造函数,需要传入channel

```
class UserServiceRpc_Stub : public UserServiceRpc {
 public:
  UserServiceRpc_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel);
  UserServiceRpc_Stub(::PROTOBUF_NAMESPACE_ID::RpcChannel* channel,
                   ::PROTOBUF_NAMESPACE_ID::Service::ChannelOwnership ownership);
  ~UserServiceRpc_Stub();

  inline ::PROTOBUF_NAMESPACE_ID::RpcChannel* channel() { return channel_; }

  // implements UserServiceRpc ------------------------------------------

  void Login(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done);
  void GetFriendLists(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                       const ::fixbug::GetFriendListsRequest* request,
                       ::fixbug::GetFriendListsResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::PROTOBUF_NAMESPACE_ID::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(UserServiceRpc_Stub);
};
```

去看该方法实现的时候 会发现

```
void UserServiceRpc_Stub::Login(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                              const ::fixbug::LoginRequest* request,
                              ::fixbug::LoginResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}
void UserServiceRpc_Stub::GetFriendLists(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                              const ::fixbug::GetFriendListsRequest* request,
                              ::fixbug::GetFriendListsResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(1),
                       controller, request, response, done);
}
```

调用函数的时候 , 会自动 调用 channel_->CalllMethod()

我们去看下 channel 是什么类

```
class PROTOBUF_EXPORT RpcChannel {
 public:
  inline RpcChannel() {}
  virtual ~RpcChannel();

  // Call the given method of the remote service.  The signature of this
  // procedure looks the same as Service::CallMethod(), but the requirements
  // are less strict in one important way:  the request and response objects
  // need not be of any specific class as long as their descriptors are
  // method->input_type() and method->output_type().
  virtual void CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done) = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RpcChannel);
};
```

会发现 channel 是一个抽象类 我们必须 继承实现该类

在方法中实现对象的序列化 及网络传输等等

method 为方法

controller

request 参数对象

response 响应对象

```
class mychannel:public RpcChannel
{
    void callmeMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done)
    {

  

    }
}
```

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
