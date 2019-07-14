#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<windows.h>
#include<WinSock2.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main()
{
	//启动Windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	// 1. 建立一个socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		cout << "socket建立失败。。。" << endl;
	}
	else
	{
		cout << "socket建立成功。。。" << endl;
	}
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(sock, (sockaddr*)&sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		cout << "连接服务器失败。。。" << endl;
	}
	else 
	{
		cout << "连接服务器成功。。。" << endl;
	}
	while (true)
	{
		char buf[1024] = {};
		//scanf("%s", buf);
		cin >> buf;
		send(sock, (const char *)&buf, sizeof(buf), 0);
		char recvBuf[1024] = {};
		recv(sock, (char*)&recvBuf, sizeof(recvBuf), 0);
		cout << "接收到消息：" << recvBuf << endl;
	}
	closesocket(sock);
	//清除Windows socket环境
	WSACleanup();
	printf("已退出。");
	getchar();
	return 0;
}