
#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"
#include <string>
#include <iostream>
 
using namespace std;
DWORD WINAPI ClientThread(LPVOID);
bool check_pass(char username[], char password[]);
char *connection_info(struct sockaddr_in &client);
SOCKET clients[64];
char *ids[64];
int numClients;
char *token[128];

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);

	numClients = 0;
	while (true)
	{
		struct sockaddr_in client_info = { 0 };
		int size = sizeof(client_info);
		SOCKET client = accept(listener, (sockaddr*)&client_info, &size);
		printf("Accepted client: %d\n", client);
		char *ipClient = connection_info(client_info);
		printf("Client IP: %s \n", ipClient);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}

	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET *)lpParam;

	char buf[1024];
	char sendBuf[256];
	int ret;

	char cmd[64];
	char id[64];
	char tmp[64];

	char targetId[64];

	char *errorMsg = "Loi cu phap. Hay nhap lai\n";

	while (true)
	{
		ret = recv(client, buf, sizeof(buf), 0);

		if (ret <= 0) break;
		buf[ret] = 0;
		printf("Received: %s\n", buf);
		if(strncmp(buf, "GET / HTTP", 10 ) == 0){
			printf("da nhan request\n");
			FILE *f = fopen("Login.html", "rb");
			while (true)
			{
				ret = fread(buf, 1, sizeof(buf), f);
				if (ret > 0)
					send(client, buf, ret, 0);
				else
					break;
			}
			closesocket(client);
			fclose(f);
		}
		else if (strncmp(buf, "POST /log-in", 12) == 0) {
			printf("da nhan request POST\n");
			printf("%s", buf);
			char *body = strstr(buf, "username=");
			char *msg;
			printf("\nbody: %s", body);
			char re_username[128], *username;
			char re_password[128], *password,end[128];
			sscanf(body, "%128[^&] & %128[^&] & %s",re_username,re_password,end);
			username = re_username + 9;
			password = re_password + 9;
			printf("\n user nhap: %s pass nhap: %s\n", username, password);
			bool a = check_pass(username, password);
			if(a){
			  msg = "HTTP/1.1 200 OK\r\n Set-Cookie: Login=true\r\nContent-Type: text/html\r\n\r\n<html><body>Dung mat khau r</body></html>";
			}
			else {
			  msg = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Sai mat khau ,ngu!!!r</body></html>";
			}
			
			send(client, msg, strlen(msg), 0);
			closesocket(client);
		}
		else if (strncmp(buf, "GET /sign-up", 12) == 0) {
			FILE *f = fopen("sign-up.html", "rb");
			while (true)
			{
				ret = fread(buf, 1, sizeof(buf), f);
				if (ret > 0)
					send(client, buf, ret, 0);
				else
					break;
			}
			closesocket(client);
			fclose(f);
		}
		else if (strncmp(buf, "POST /sign-up", 13) == 0) {
			// lay du lieu o day roi ghi vao file data.txt,nho check user da ton tai hay cgya


			char *msg = "HTTP/1.1 200 OK\r\n Set-Cookie: Login=true\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Dang ki thanh cong </h1> <a href='/'><button>Back</button></a> </body></html>";
			send(client, msg, strlen(msg), 0);
			closesocket(client);
		}
		
	}
	closesocket(client);
}
char *connection_info(struct sockaddr_in &client)
{
	char *connected_ip = inet_ntoa(client.sin_addr);
	int port = ntohs(client.sin_port);
	return connected_ip;
}

bool check_pass(char username[], char password[]) {
	FILE *f = fopen("data.txt", "rb");
	int ret;
	char buf[1024];
	char userDb[128], passDb[128], end[128];
	int i = 0;
	int count = 0;
	while (true)
	{
		ret = fread(buf, 1, sizeof(buf), f);
		buf[ret] = 0;
		if (ret > 0) {
			printf("buf: %s", buf);
			for (int i = 0; i < ret + 2; i++) {
				char *lineInText;
				if (buf[i] == '\n' || i == ret + 1) {
					lineInText = buf + count;
					lineInText[i - count - 1] = 0;
					if (i == ret + 1) {
						lineInText = buf + count;
					}
					count = i + 1;
					int slpit;
					slpit = sscanf(lineInText, "%s %s %s", userDb,passDb, end);
					printf("\n user: %s pass: %s", userDb, passDb);
				
					if (strcmp(username, userDb) == 0 && strcmp(password, passDb) == 0) {
						printf("dung mat khau");
						return true;
					}
				}

			}
		}
		else
			break;
	}
	fclose(f);
	return false;
}

char *generateToken() {
	int i = 0;
	char token[10];
	while (i < 10) {

		i++;
	}
	return token;
}