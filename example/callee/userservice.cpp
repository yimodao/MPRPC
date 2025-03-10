#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class UserService:public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name,std::string pwd) //使用在rpc服务发布端
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd <<std::endl;
        return true;
    }
    void Login(::google::protobuf::RpcController* controller,
        const ::fixbug::LoginRequest* request,
        ::fixbug::LoginResponse* response,
        ::google::protobuf::Closure* done) 
    {
        //框架给业务上报了请求LoginRequest，应用获取相应数据做本地应用
        std::string name = request->name();
        std::string pwd = request->pwd();
        std::cout << "recieve name :" << name.c_str() << " pwd:" << pwd.c_str();
        bool login_result = Login(name,pwd); //做本地业务
        //把响应写入
        response->set_success(login_result);
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        //执行回调操作 执行返回对象序列化并发送（由框架来完成）
        done->Run();
    }
};

int main(int argc, char** argv)
{
    //调用框架的初始化操作 provider -i config.conf
    MprpcApplication::Init(argc,argv);
    //provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService);
    //启动一个rpc服务发布节点，Run以后进程进入阻塞，等待远程rpc调用请求
    provider.Run();
    return 0;
}