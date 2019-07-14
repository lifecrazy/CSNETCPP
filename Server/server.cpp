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
	// ����windos socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	// ��socket API��������TCP�����
	// 1.����һ��socket�׽���
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 2.bind �����ڽ��տͻ������ӵ�����˿�
	sockaddr_in sin = {};
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888); //host to net unsigned short
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //����inet_addr("127.0.0.1")
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&sin, sizeof(sin)))
	{
		cout << "����˿ڰ�ʧ�ܡ�����" << endl;
	}
	else
	{
		cout << "�˿ڣ�" << sin.sin_port << "�󶨳ɹ�������" << endl;
	}
	// 3.listen ��������˿�
	if (SOCKET_ERROR == listen(sock, 5)) 
	{
		cout << "����˿ڼ���ʧ�ܡ�����" << endl;
	}
	else
	{
		cout << "�˿ڣ�" << sin.sin_port << "�����С�����" << endl;
	}
	// 4. accept �ȴ����տ�̶�����
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET cSock = INVALID_SOCKET;
	cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == cSock) 
	{
		cout << "���յ���Ч�ͻ���SOCKET������" << endl;
	}
	cout << "�¿ͻ��˼��룺socket = " << (int)cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) << endl;
	char buf[1024] = {};
	while (true)
	{
		// 5.���տͻ�������
		//DataHeader header = {};
		int nLen = recv(cSock, (char*)&buf, sizeof(buf), 0);
		if (nLen <= 0) 
		{
			cout << "�ͻ������˳������������" << endl;
			break;
		}
		cout << "���յ���" << buf << endl;
		char str[1024] = "���ճɹ�";
		send(cSock, (char*)&str, sizeof(str) , 0);
	}
	// 8 �ر��׽��� closesocket
	closesocket(sock);
	// ���windodws socket ����
	WSACleanup();
	cout << "���˳���" << endl;
	getchar();
	return 0;
}