// FTP_1.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <process.h>
#include <conio.h>
#include <time.h>
#include "TCPIP_Sync.cpp"


char random_c();

char	IP[100] = "140.113.9.151",NLST_IP[100];	//NCTU FTP Server IP
int		Port = 21;					//FTP Port
SOCKET	Sock,NLST_Sock;						//訓令通道
int		NLST_Flag=0,NLST_Port;

void Parser(char *S,char *IP,int *Port1)//解析IP+Port
{
	int i = 0,begin=0,end=0,comma=0,tag;
	int i1, i2, i3, i4, i5, i6;
	char S2[100];
	for (i = 0;i < strlen(S);i++)
	{
		if (S[i] == '(') begin = i + 1;
		else if (S[i]==')')  end = i-1;
	}
	if (end > begin)
	{
		strncpy_s(S2, &S[begin], end - begin +1);
		sscanf_s(S2,"%d,%d,%d,%d,%d,%d", &i1, &i2, &i3, &i4, &i5, &i6);
		sprintf_s(IP,100, "%d.%d.%d.%d", i1, i2, i3, i4);
		*Port1 = i5 * 256 + i6;
		//printf("Get:[IP]%s:[Port]%d\r\n",IP,*Port1);
	}
}

void NLST_fun(PVOID p)
{
	int		i;
	char	S1[2000];
	while (1)
	{
		i = recv(NLST_Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0)
		{
			S1[i] = 0;  printf("%s", S1);
		}
	}
}

void fun(PVOID p)					//執行序 Receive DATA
{
	int		i;
	char	S1[2000];
	while (1)
	{
		i = recv(Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0) 
		{
			S1[i] = 0;  printf("%s", S1);
			if (NLST_Flag)//NLST=PASV(Done in main)+Start_TCP_Client+NLST
			{
				Parser(S1, NLST_IP, &NLST_Port);
				Start_TCP_Client(&NLST_Sock, NLST_Port, NLST_IP);
				_beginthread(NLST_fun, 0, 0);
				send(Sock, "NLST\r\n", 6, 0);
				NLST_Flag = 0;
			} 
		}
	}
}

int main()
{
	int Cur = 0, Cur1 = 0;
	char c,Account[100],Password[100],S[2000];

	system("chcp 65001");
	system("color a");

    //Activate TCP & Connect to Server
	Start_TCP_Client(&Sock,Port,IP);
	_beginthread(fun, 0, 0);
	//Sleep(10000);	//Kirito:幫我撐10秒(#
	//system("PAUSE");//C8763
	
	//Input Account
	Sleep(1000);
	printf("Account:");scanf_s("%s",Account,sizeof(Account));
	sprintf_s(S, "USER %s\r\n", Account);
	send(Sock, S, strlen(S), 0);
	
	
	//Input Password
	Sleep(1000);
	printf("password:"); 
	do
	{
		//c = random_c();
		Password[Cur++] = _getch();//printf("%c", c);
	} while (Password[Cur-1]!=0x0d);
	Password[--Cur] = 0;
	printf("\n");
	//scanf_s("%s", Password, sizeof(Password));
	sprintf_s(S, "PASS %s\r\n", Password);
	send(Sock, S, strlen(S), 0);
	
	//Enter Command Mode
	while (1)
	{
		Sleep(1000);
		printf("FTP>"); //scanf_s("%s", S, sizeof(S));
			Cur1 = 0;
			do
			{
				//c = random_c();
				S[Cur1++] = _getche();//printf("%c", c);
			} while (S[Cur1 - 1] != 0x0d);
			S[Cur1 - 1] = 0;
			printf("\r\n");
		//Translate Commands NLST(列印檔案目錄),(RETR,STOR)
		if (!strncmp(S, "NLST", 4))
		{	
			NLST_Flag = 1;
			strcpy_s(S, "PASV");
		}
		//Send the Commands	
		strcat_s(S,"\r\n");
		send(Sock, S, strlen(S), 0);
	}

}

char random_c()
{

	int a;
	char c;
	srand(time(NULL));
	a = ((rand()*87638763) % 92)+33;
	c = a;
	return c;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
