#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<Windows.h>
#include<WinSock2.h>
#include<iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
struct DataHeader
{
	short dataLength;
	short cmd;
};
// DataPackage
struct Login :public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
};
struct LoginResult :public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
struct Logout :public DataHeader 
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
struct LogoutResult :public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
int main()
{
	// 启动windos socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// 用socket API建立简易TCP服务端
	// 1.建立一个socket套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 2.bind 绑定用于接收客户端连接的网络端口
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888); //host to net unsigned short
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //或者inet_addr("127.0.0.1")
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&sin, sizeof(sin)))
	{
		cout << "网络端口绑定失败。。。" << endl;
	}
	else
	{
		cout << "端口：" << sin.sin_port << "绑定成功。。。" << endl;
	}
	// 3.listen 监听网络端口
	if (SOCKET_ERROR == listen(sock, 5)) 
	{
		cout << "网络端口监听失败。。。" << endl;
	}
	else
	{
		cout << "端口：" << sin.sin_port << "监听中。。。" << endl;
	}
	// 4. accept 等待接收库短短连接
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET cSock = INVALID_SOCKET;
	cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == cSock) 
	{
		cout << "接收到无效客户端SOCKET。。。" << endl;
	}
	cout << "新客户端加入：socket = " << (int)cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) << endl;
	char buf[1024] = {};
	while (true)
	{
		// 5.接收客户端数据
		//DataHeader header = {};
		int nLen = recv(cSock, (char*)&buf, sizeof(buf), 0);
		if (nLen <= 0) 
		{
			cout << "客户端已退出，任务结束。" << endl;
			break;
		}
		cout << "接收到：" << buf << endl;
		char str[1024] = "接收成功";
		send(cSock, (char*)&str, sizeof(str) , 0);
	}
	// 8 关闭套接字 closesocket
	closesocket(sock);
	// 清除windodws socket 环境
	WSACleanup();
	cout << "已退出。" << endl;
	getchar();
	return 0;
}