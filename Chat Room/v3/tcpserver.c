#include "mylib.h"

#define MAX_CLIENT 1024

int client[MAX_CLIENT];
int cnum = 0;

void* client_process(void *arg)
{
	pthread_detach(pthread_self());
	int sockfd = *((int*)arg);
	char* welcome = "Hay gui lenh theo cu phap \"client_id: <ten>\"\n";
	writen(sockfd, welcome, strlen(welcome));
	
	char buffer[1024];
	char name[1024];
	int n;
	int temp;
	
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		n = read(sockfd, buffer, sizeof(buffer));
		if (n == 0) return NULL;
		if(strncmp(buffer, "client_id: ", 11) == 0)
		{
			memset(name, 0, sizeof(name));
			strcpy(name, buffer + 11);
			name[strlen(name) - 1] = 0;
			int i = 0;			
			for(; i < MAX_CLIENT; i++)
			{
				if(client[i] == 0)
				{
					client[i] = sockfd;
					break;
				}
			}
			if(i > MAX_CLIENT) 
			{
				char* cannotConnect = "Server qua tai!!!\n";
				writen(sockfd, cannotConnect, strlen(cannotConnect));
				return NULL;
			}
			temp = i;
			if(i > cnum) 
				cnum = i;
			char* invalid = "CHUC MUNG BAN DA VAO ROOM\n";
			writen(sockfd, invalid, strlen(invalid));
			printf("A client has connected\n");
			break;
		}
		else
		{
			char* invalid = "Sai cu phap\n";
			writen(sockfd, invalid, strlen(invalid));
		}
	}

	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		if( (n = read(sockfd, buffer, sizeof(buffer))) > 0)
		{
			char message[1024];
			sprintf(message, "%s: %s", name, buffer);
			for(int k = 0; k <= cnum; k++)
			{
				if(client[k] != 0 && client[k] != sockfd)
				{
					writen(client[k], message, strlen(message));
				}
			}
		}
		else if(n == 0) break;
	}
	client[temp] = 0;
	printf("A client has disconnected\n");
	return NULL;
}

int main(int argc, char** argv)
{
	int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	
	while(1)
	{
		struct sockaddr_in cliaddr;
		socklen_t clen = sizeof(cliaddr);
		int* connfd = (int*)calloc(1, sizeof(int));		
		*connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clen);
		pthread_t tid;
		pthread_create(&tid, NULL, &client_process, connfd);		
	}
}
