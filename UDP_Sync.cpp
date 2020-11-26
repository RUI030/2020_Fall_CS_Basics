#include <iostream>
#include <process.h>
#include "TCPIP_sync.cpp"

 
#define ip (char *)"192.168.13.255"

SOCKET		ssock, csock;
sockaddr_in saddr, caddr;
void fun(PVOID p)//Receive UDP Server messages 執行緒子程式
{
	int i,len = sizeof(sockaddr);
	char s1[2000];
	while (1)
	{
		i = recvfrom(ssock, s1, sizeof(s1) - 1, 0, (sockaddr *)&saddr, &len);
		if (i > 0)
		{

			s1[i] = 0; printf("[%3d.%3d.%3d.%3d]>>>%s\n\r", (saddr.sin_addr.s_addr>>0)&0xFF, (saddr.sin_addr.s_addr >> 😎 & 0xFF, (saddr.sin_addr.s_addr >> 16) & 0xFF, (saddr.sin_addr.s_addr >> 24) & 0xFF, s1);
		}
	}
}

int main()
{
	system("color a");

	//Startup UDP Server & Activate Thread to Receive DATA
	Start_UDP_Server(&ssock, 8763);

	_beginthread(fun, 0, 0);

	//Startup UDP Client
	Start_UDP_Client(&csock, &caddr, ip, 8763);

	//Read keyboard & Send DATA
	char s1[2000];
	int len = sizeof(sockaddr);
	while (1)
	{
		scanf_s("%s", s1, sizeof(s1));
		sendto(csock, s1, strlen(s1), 0, (sockaddr *)&caddr, len);
	}
}
