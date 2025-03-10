#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/base/Timestamp.h"
#include <google/protobuf/descriptor.h>
#include <unordered_map>

//框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider
{
public:
    //这是框架提供给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service* service);
    //启动rpc服务节点，开始提供远程的rpc调用请求

    void Run();
private:
    muduo::net::EventLoop loop;
    //服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service* m_service;//保存对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; //保存服务方法

    };
    //存储注册成功的服务对象和其方法的信息。
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;
    std::unique_ptr<muduo::net::TcpServer> server_Ptr;
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    void OnMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer* buffer,muduo::Timestamp receiveTime);
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message *response);
};