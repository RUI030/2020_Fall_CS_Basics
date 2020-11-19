#include <iostream>

//1.Set up
#pragma comment(lib,"ws2_32.lib")
#include "winsock2.h"

int main()
{
	system("color a");
	//2.Open socket
	WSADATA wsa;	//Windows Socket API
	SOCKET sock;
	sockaddr_in addr;
	char s1[2000];
	int len = sizeof(sockaddr);
	WSAStartup(0x202, &wsa);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);	//port
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//
	bind(sock, (sockaddr *)&addr, len);	//綁定port , Activate server
	//Receive & Print DATA
	sockaddr addr1;
	int i;
	while (1)
	{
		i = recvfrom(sock, s1, sizeof(s1) - 1, 0, &addr1, &len);
		sendto(sock, s1, strlen(s1), 0, (sockaddr *)&addr, len);
		if (i > 0)
		{
			s1[i] = 0; printf(">>> %s\n", s1);
		}
	}
}
