#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

// =======================================================================
// ==================  啟動 UDP Client   =================================
// =======================================================================
void Start_UDP_Client(SOCKET *Socket, sockaddr_in *lpAddr, char *IP, int Port)
{
	WSADATA     wsadata;
	int IP1, IP2, IP3, IP4;
	WSAStartup(0x202, &wsadata);            // 啟動Winsock
	*Socket = socket(AF_INET, SOCK_DGRAM, 0);  // TCP socket open
	lpAddr->sin_family = AF_INET;
	lpAddr->sin_port = htons(Port);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	lpAddr->sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);
}
// ==================================================
// ==================  啟動 UDP Server   ============
// ==================================================
void Start_UDP_Server(SOCKET *Socket, int Port)
{
	WSADATA      wsadata;
	sockaddr_in  Addr;

	// 1. 啟動 UDP Server
	WSAStartup(0x202, &wsadata);
	*Socket = socket(AF_INET, SOCK_DGRAM, 0);
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(*Socket, (sockaddr *)&Addr, sizeof(sockaddr));
}


// ==================  啟動 TCP Server(使用thread接受連線與接收資料)   ===
int  Start_TCP_Server(SOCKET *psockfd, WORD Port)
{
	WSADATA  wsadata; int      err;
	struct   sockaddr_in  tcpserver;
	// 1. 開啟 TCP Server
	if ((err = WSAStartup(0x202, &wsadata)) != 0) return -1;
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr *)&tcpserver, sizeof(tcpserver)) < 0) return -1;
	if ((err = listen(*psockfd, SOMAXCONN)) < 0) return -1;
	return 0;
}

// ==================  啟動 TCP Client   =================================
int Start_TCP_Client(SOCKET *psockfd, WORD R_Port, char *IP)
{
	WSADATA  wsadata;
	int      err;
	sockaddr_in  tcpclient;
	int IP1, IP2, IP3, IP4;

	if ((err = WSAStartup(0x202, &wsadata)) != 0) return -1;
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(R_Port);
	//tcpclient.sin_addr.s_addr=inet_addr(IP);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	tcpclient.sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);


	if (err = connect(*psockfd, (sockaddr *)&tcpclient, sizeof(tcpclient)) < 0) return -1;
	return 0;
}
