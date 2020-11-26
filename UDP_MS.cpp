#include <iostream>
#include "TCPIP_Sync.cpp"

SOCKET SSock;

int main()
{
	//Declare
	int i,Port, Len = sizeof(sockaddr);
	sockaddr_in Addr;
	char S1[2000],IP[100];

    //1. Start up UDP Server
	Start_UDP_Server(&SSock, 6000);

	//2. Receive DATA & Recognize USER & Pass DATA
	while (1)
	{
		// a.Receive DATA
		i = recvfrom(SSock, S1,sizeof(S1) - 1, 0, (sockaddr*)&Addr, &Len);
		// b.Recognize USER
		
		sprintf_s(IP, sizeof(IP), "%3d.%3d.%3d.%3d",
				(Addr.sin_addr.s_addr >> 0) & 0xFF,
				(Addr.sin_addr.s_addr >> 8) & 0xFF,
				(Addr.sin_addr.s_addr >> 16) & 0xFF,
				(Addr.sin_addr.s_addr >> 32) & 0xFF);
		Port = Addr.sin_port;
		Port = (Port>>8&0xFF) + (Port & 0xFF) << 8 ;

		if (i > 0) { S1[i] = 0;printf("{PORT:%d}[USER:%s] > %s\n\r",Port,IP, S1); }
		// c.Sent DATA

		// d.Show DATA
		
	}
}
