//客户端
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>//sockaddr_in头文件
#include <arpa/inet.h>//inet_addr头文件
#include <string.h>
#include <unistd.h> //close函数头文件

#define SVRPORT 8887

int main()
{
    int iClientSkFd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in stSvrAddr;
    stSvrAddr.sin_family = AF_INET;
    stSvrAddr.sin_port = htons(SVRPORT);
    stSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int iRecTag = connect(iClientSkFd ,(struct sockaddr *)&stSvrAddr , sizeof(stSvrAddr));
    printf("iClientFd %d\n",iClientSkFd);
    printf("iRecTag %d\n",iRecTag);

    if (iRecTag == -1)
    {
        printf("连接错误");
        exit(0);
    }

    char szSendBuf[256] = "fuuuuuu";
    char szRecBuf[256] = {};
    while(true)
    {
        //send(iClientSkFd , szSendBuf , strlen(szSendBuf), 0);
        read(iClientSkFd , szRecBuf , 256);
        printf("recv str %s\n" , szRecBuf);
    }
    close(iClientSkFd);
    return 0;
}