#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main()
{
	char buffer[256];
	int result,nread;
	fd_set inputs,testfds;
	struct timeval timeout;

	FD_ZERO(&inputs); //初始化为空集合
	FD_SET(0,&inputs); //在集合中设置由参数fd(此处初始为0，即为标准输入)传递的文件描述符

	while(1)
	{
		testfds = inputs;     //暂时不大了解这语句的作用，怎么要设置testfds和inputs，谁可以指导我一下
		timeout.tv_sec=3;
		timeout.tv_usec=500000;     //设置超时时限为3.5秒
        //如果3.5秒内没有输入数据，则会再次循环
		result = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set*)NULL, &timeout);

		switch(result)
		{
		case 0:     //超时，select函数返回0
			printf("select called is timeout.\n");
			break;
		case -1:   //select系统调用遇到错误，函数返回-1
			perror("select called encounterd errors.\n");
			exit(1);
		default:       //文件描述符状态发生变化，系统调用read进行数据读取
			if(FD_ISSET(0, &testfds))
			{
				ioctl(0, FIONREAD, &nread);  //设置标准输入stdin
				if(nread==0)
				{
					printf("keyboard done.\n");
					exit(0);
				}
				nread = read( 0 , buffer , nread);
				//write(0, "fuck", 4);    //读取数据
				buffer[nread] = 0;
				printf("read %d from keyboard: %s", nread , buffer);
			}
			break;
		}

	}

	return 0;
}

