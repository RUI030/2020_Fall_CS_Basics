//搭配TCP_Client.cpp服用

#include <iostream>
#include "TCPIP_Sync.cpp"
#include <Windows.h>

#define MAX 1000		//容納1000人
SOCKET	CSock[MAX];		//紀錄終端(Client)連線Sock
SOCKET	SSock;			//TCP Server Sock
int		CNum = 0;		//終端連線人數
char	Msg[MAX][2000];	//紀錄訊息

void Print_DATA()
{
	system("cls");
	int i;
	for (i = 0;i < CNum;i++)
	{
		printf("<%02d>(Sock:%5d) | Msg:%s\n", i, CSock[i], &Msg[i][0]);
	}
}

int main()
{
	//	1.啟動TCP Server
	for (int i = 0;i < MAX ;i++)strcpy_s(&Msg[i][0],2000,"None");
	fd_set fdSock, fdRead;
	Start_TCP_Server(&SSock,6000);
	FD_ZERO(&fdSock);	FD_SET(SSock,&fdSock);			//將SSock推入Pool
	printf("[TCP伺服器啟動完畢]\r\n");

	//	2.檢查網路連線狀態		 -> select					(epoll)
	int			i,j,Len=sizeof(sockaddr);
	char		S1[2000];
	SOCKET		wParam;
	sockaddr	Addr;
	while (1)
	{
		fdRead = fdSock;								//備份:fdSock不要更改
		select(0, &fdRead, 0, 0, 0);
		for (i=0;i<fdSock.fd_count;i++)
		{
			wParam = fdSock.fd_array[i];				//取出有動靜之Socket
			//	2.1 遠端連入		 -> accept()
			if (wParam == fdRead.fd_array[0])
			{
				if (wParam == SSock)
				{
					CSock[CNum] = accept(wParam, &Addr, &Len);
					strcpy_s(&Msg[CNum][0], 2000, "[Connected]");
					FD_SET(CSock[CNum], &fdSock);			//將CSock推入Pool
					CNum++;
				}
				//	2.2.1 遠端送來資料	 -> recv()
				else
				{
					i = recv(wParam, S1, sizeof(S1) - 1, 0);
					for (j = 0;j < CNum;j++) if (wParam == CSock[j])break;
					if (i >= 0) {S1[i] = 0; strcpy_s(&Msg[j][0], 2000, S1);}
					//	2.2.2 遠端斷線 -> closesocket()
					else 
					{
						closesocket(wParam);
						FD_CLR(wParam, &fdSock);
						strcpy_s(&Msg[j][0], 2000, "[Disconnected]");
					}
				}
				Print_DATA();
			}
		}
	}
	return 0;
}
