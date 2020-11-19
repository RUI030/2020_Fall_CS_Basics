#include <iostream>

#pragma comment(lib,"ws2_32.lib")
#include "winsock2.h"

int main()
{
	//2.Open socket
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in addr;
	char s1[2000];
	int len = sizeof(sockaddr);
	WSAStartup(0x202, &wsa);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = (xxx << 24) + (xxx << 16) + (xxx << 8) + xxx;  //ip

	while (1)
	{
		printf(">"); scanf_s("%s", s1, sizeof(s1));
		sendto(sock, s1, strlen(s1), 0, (sockaddr *)&addr, len);

	}
}
