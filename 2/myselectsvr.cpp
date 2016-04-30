//利用io模型实现单个服务器 和 多个客户端的通信
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in头文件
#include <arpa/inet.h>//inet_addr头文件
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ErrorPrint(error) (printf("%s\n" , error))

#define SVR_PORT 8887
#define MAX_BUF_LOG 5
#define MAX_FD_NUMS 100

int szClientFdSet[MAX_FD_NUMS];

int main()
{
    int iSvrSocketFd = socket(AF_INET , SOCK_STREAM , 0);
    sockaddr_in stSvrAddr;
    stSvrAddr.sin_family = AF_INET;
    stSvrAddr.sin_port = htons(8887);
    stSvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(iSvrSocketFd , (struct sockaddr *)(&stSvrAddr) , sizeof(stSvrAddr)))
    {
        ErrorPrint("绑定出错");
        exit(0);
    }

    if (listen(iSvrSocketFd , MAX_BUF_LOG))
    {
        ErrorPrint("监听出错");
        exit(0);
    }

    sockaddr_in stClientAddr;
    socklen_t  iClientAddrLn = sizeof(stClientAddr);

    int iClientFdSetLn = 0;
    memset(szClientFdSet , -1 , sizeof(szClientFdSet));

    struct fd_set stFdSet , stTmpSet , stWriteSet;
    FD_ZERO(&stFdSet);
    FD_SET(iSvrSocketFd , &stFdSet);
    int maxFd = iSvrSocketFd;
    struct timeval stTime;
    stTime.tv_sec = 2;
    stTime.tv_usec = 0;

    char szBuf[256] = "fuck client";
    //当有客户端 连接服务器时 服务器的套接字是可读的
    for ( ; ; )
    {
        stTmpSet = stFdSet;
        int iStatus = select(maxFd + 1 , &stTmpSet , &stWriteSet , (struct fd_set *)NULL , &stTime);
        printf("%d %d\n", iClientFdSetLn , iStatus);
        switch(iStatus)
        {
            case -1: ErrorPrint("select 返回的状态出错");exit(0);
            case 0:
            for (int i = 0 ; i <iClientFdSetLn ; ++i)
                {
                    //if (FD_ISSET(szClientFdSet[i], &stTmpSet))
                    //{
                        //printf("write write");
                        //int iReadLn = read(szClientFdSet[i] , szBuf , 256);
                        //szBuf[iReadLn] = 0;
                        //send(szClientFdSet[i] , szBuf , strlen(szBuf) , 0);
                        write(szClientFdSet[i] , szBuf , strlen(szBuf));
                    //}
                }
            break;
            default:
                if (FD_ISSET(iSvrSocketFd , &stTmpSet))
                {
                    int iClientFD = accept(iSvrSocketFd , (struct sockaddr *)(& stClientAddr) , &iClientAddrLn);
                    printf("fuck3");
                    if (iClientFD > 0 )
                    {
                        printf("new cliet connect , the fd is %d" , iClientFD);
                        FD_SET(iClientFD , &stFdSet);
                        szClientFdSet[iClientFdSetLn++] = iClientFD;
                        if (iClientFD > maxFd)
                        {
                            maxFd = iClientFD;
                        }
                    }
                }
            break;
        }
    }
    return 0;
}
