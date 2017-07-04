#include "mylib.h"
#include <limits.h>

struct pollfd client[OPEN_MAX];
char* name[OPEN_MAX];
uint8_t client_status[OPEN_MAX];

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for(int i = 1; i < OPEN_MAX; i++)
	{
		client[i].fd = -1;
	}
	int maxi = 0;
	while(1)
	{
		int nready = poll(client, maxi + 1, -1);
		
		if(client[0].revents & POLLRDNORM)
		{
			struct sockaddr_in cliaddr;
			socklen_t len = sizeof(cliaddr);
			int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			int i;			
			for(i = 1; i < OPEN_MAX; i++)
			{
				if (client[i].fd < 0)
				{
					client[i].fd = connfd;
					break;
				}
			}
			if(i == OPEN_MAX)
			{
				char* errorConnect = "Khong the ket noi toi server!!!\n";
				writen(connfd, errorConnect, strlen(errorConnect));
				continue;
			}
			client[i].events = POLLRDNORM;
			char* welcome = "Gui lenh theo cu phap \"client_id: <ten>\"\n";
			writen(connfd, welcome, strlen(welcome));	
			if(i > maxi) maxi = i;
			if(--nready <= 0)
				 continue;
		}

		for(int i = 1; i <= maxi; i++)
		{
			if(client[i].fd < 0)
				continue;
			if(client[i].revents & POLLRDNORM)
			{
				char buffer[1024];
				if(readline(client[i].fd, buffer, sizeof(buffer)) == 0)
				{
					close(client[i].fd);
					client[i].fd = -1;
					client_status[i] = 0;
					free(name[i]);
					printf("1 client da thoat!!!\n");
					continue;
				}
				if(client_status[i] == 0)
				{
					if(strncmp(buffer, "client_id: ", 11) == 0)
					{
						char* valid = "CHUC MUNG BAN DA VAO ROOM\n";
						writen(client[i].fd, valid, strlen(valid));
						client_status[i] = 1; //da vao room
						name[i] = (char*)calloc(MAXLINE, 1);	
						strcpy(name[i], buffer + 11);
						name[i][strlen(name[i]) - 1] = 0;
					}
					else
					{
						char* invalid = "SAI CU PHAP\n";
						writen(client[i].fd, invalid, strlen(invalid));
					}
				}
				else 
				{
					char message[1024];
					sprintf(message, "%s: %s", name[i], buffer);
					for(int k = 1; k <= maxi; k++)
					{
						if(client_status[k] == 1 && k != i)
						{
							writen(client[k].fd, message, strlen(message));
						}
					}
				}
				if(--nready <= 0)
					break;
			}
		}
	}
}
