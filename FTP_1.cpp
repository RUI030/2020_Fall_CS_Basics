// FTP_1.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <process.h>
#include <conio.h>
#include <time.h>
#include "TCPIP_Sync.cpp"


char random_c();
//==FTP Command Channel==
SOCKET	Sock;						//訓令通道
//char	IP[100] = "140.113.9.151";	//NCTU FTP Server IP
char	IP[100] = "140.113.170.87";
int		Port = 13579;					//FTP Port					
//=========NLST==========
SOCKET  NLST_Sock;						
char	NLST_IP[100];
int		NLST_Port;
int		NLST_Flag = 0;
//=======RETR a.c========
SOCKET  RETR_Sock;					//DL 通道
char	RETR_IP[100];
int		RETR_Port;
int		RETR_Flag = 0;
char	RETR_File[100];
//=======STOR a.c========
SOCKET  STOR_Sock;					//Upload 通道
char	STOR_IP[100];
int		STOR_Port;
int		STOR_Flag = 0;
char	STOR_File[100];

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

void RETR_fun(PVOID p)				//執行緒:DL通道,Receive File 內容
{
	FILE *out;
	errno_t err=fopen_s(&out,RETR_File, "wb");
	int		i,j=0;
	char	S1[2000];
	while (1)
	{
		i = recv(RETR_Sock, S1, sizeof(S1) - 1, 0);
		
		if (i > 0)
		{
			S1[i] = 0;  
			//printf("%s", S1);
			fwrite(S1,1,i,out);
			j += i;printf("[DL] %d Bytes\n", j);
			
		}
		else
		{
			closesocket(RETR_Sock);break;
		}
		
	}
	fclose(out);
}

void NLST_fun(PVOID p)				//執行緒:DL通道,Receive File 目錄
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
		else
		{
			closesocket(NLST_Sock);break;
		}
	}
}

void STOR_fun(PVOID p)				//執行緒:Upload 通道, Upload File Content
{
	FILE	*in;
	errno_t fp = fopen_s(&in, STOR_File, "rb");
	int		i,j=0;
	char	S1[2000];
	while (!feof(in))
	{
		i=fread(S1,1,sizeof(S1),in);
		Sleep(100);j += i;printf("[Upload] %d Bytes\n",j);
		send(STOR_Sock, S1, i, 0);
	}
	closesocket(STOR_Sock);fclose(in);
}

void fun(PVOID p)					//執行緒:Receive Command DATA
{
	int		i;
	char	S1[2000];
	while (1)
	{
		i = recv(Sock, S1, sizeof(S1) - 1, 0);
		if (i > 0) 
		{
			S1[i] = 0;  printf("%s", S1);
			//1.DL File 目錄
			if (NLST_Flag)//NLST=PASV(Done in main)+Start_TCP_Client+NLST
			{
				Parser(S1, NLST_IP, &NLST_Port);
				Start_TCP_Client(&NLST_Sock, NLST_Port, NLST_IP);
				_beginthread(NLST_fun, 0, 0);
				send(Sock, "NLST\r\n", 6, 0);
				NLST_Flag = 0;
			} 
			//2.DL File Content
			if (RETR_Flag)//RETR a.c=PASV(Done in main)+Start_TCP_Client+RETR a.c
			{
				Parser(S1, RETR_IP, &RETR_Port);
				Start_TCP_Client(&RETR_Sock, RETR_Port, RETR_IP);
				_beginthread(RETR_fun, 0, 0);
				sprintf_s(S1,"RETR %s\r\n",RETR_File);
				send(Sock, S1, strlen(S1), 0);
				RETR_Flag = 0;
			}
			//3.Upload File Content
			if (STOR_Flag)//STOR a.c=PASV(Done in main)+Start_TCP_Client+STOR a.c
			{
				Parser(S1, STOR_IP, &STOR_Port);
				Start_TCP_Client(&STOR_Sock, STOR_Port, STOR_IP);
				_beginthread(STOR_fun, 0, 0);
				sprintf_s(S1, "STOR %s\r\n", STOR_File);
				send(Sock, S1, strlen(S1), 0);
				STOR_Flag = 0;
			}
		}
		else
		{
			closesocket(Sock);break;
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
	printf("Password:"); 
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
		else if (!strncmp(S, "RETR", 4))
		{
			sscanf_s(S,"RETR %s",RETR_File,sizeof(RETR_File));
			//printf("[GET]RETR_File:%s\n",S);
			RETR_Flag = 1;
			strcpy_s(S, "PASV");
		}
		else if (!strncmp(S, "STOR", 4))
		{
			sscanf_s(S, "STOR %s", STOR_File, sizeof(STOR_File));
			STOR_Flag = 1;
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
