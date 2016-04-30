//简单的服务器程序(只接收一个客户端连接)
//简单服务器的几个步骤:
//创建服务端的套接字(socket , socket 是函数 返回的是套接字的句柄)
//绑定套接字到本端得ip和端口(bind)
//服务端监听端口信息(listen)
//服务端接受来自该端口的客户端连接信息请求(accept)
//服务端接受客户端发给的信息 并做处理(常用处理 send 发送 recv接受 等等其他)
#include <sys/types.h>
#include <sys/socket.h>//socket头文件
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>//sockaddr_in头文件
#include <arpa/inet.h>//inet_addr头文件
#include <string.h>
#include <unistd.h> //close函数头文件


#define SERVER_PORT 8887
#define MAXQUENUM 5


int main()
{
    //------建立套接字begin-----
    int iSocketFd = socket(AF_INET , SOCK_STREAM , 0);
    if (iSocketFd < 0){
        printf("socketFd 为%d error" , iSocketFd);
        exit(0);
    }
    //-------建立套接字end------
    
    //------绑定套接字到对应端口和地址begin-----
    struct sockaddr_in stSockAddr;
    stSockAddr.sin_family = AF_INET;              //表示协议族
    stSockAddr.sin_port = htons(SERVER_PORT);            //表示端口
    stSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //表示ip地址
    if (bind(iSocketFd , (struct sockaddr *)(&stSockAddr) , sizeof(stSockAddr) ) == -1)
    {
        printf("绑定出错");
        exit(0);
    }
    //------绑定套接字到对应端口和地址end-----
    
    //-----监听端口 等待连接队列的最大长度begin , 作用是 socket进入监听状态----
    if (listen(iSocketFd , MAXQUENUM))
    {
        printf("监听端口消息失败");
        exit(0);
    }
    //-----监听端口 等待连接队列的最大长度end----
    
    //----接受来自客户端的连接 如果没有则阻塞 返回的是客户端的套接字idbegin---
    int iClientSockFd;
    struct sockaddr_in stClientConn;
    unsigned int uSize = sizeof(stClientConn);
    iClientSockFd = accept(iSocketFd , (struct sockaddr *)(&stClientConn) , &uSize);//第三个参数是第二个字节的大小
    //----接受来自客户端的连接 如果没有则阻塞end---
    
    //----服务器逻辑操作begin------
    char szReadBuff[256] = {};
    char szWriteBuff[256] = {};
    while(true)
    {
        //没接收到会一直等待
        int iRecTag = recv(iClientSockFd , szReadBuff , sizeof(szReadBuff) , 0);
        printf("client msg : %d" , iRecTag);
        send(iClientSockFd , szReadBuff , sizeof(szReadBuff) , 0);
    }
    close(iClientSockFd);
    close(iSocketFd);
    //----服务器逻辑操作end------
    return 0;
}
