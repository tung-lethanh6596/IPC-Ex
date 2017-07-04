#include "mylib.h"

int16_t client[FD_SETSIZE];
uint8_t client_status[FD_SETSIZE];
char* name[FD_SETSIZE];

int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	
	int maxfd = listenfd;
	for(int i = 0; i < FD_SETSIZE; i++)
	{
		client[i] = -1;
	}
	
	fd_set fd_read;
	int maxi = 0;

	while(1)
	{
		FD_ZERO(&fd_read);
		FD_SET(listenfd, &fd_read);
		for(int i = 0; i <= maxi; i++)
		{
			if(client[i] > 0) FD_SET(client[i], &fd_read);
		}
		select(maxfd + 1, &fd_read, NULL, NULL, NULL);
		if(FD_ISSET(listenfd, &fd_read))
		{
			struct sockaddr_in cliaddr;
			int len = sizeof(cliaddr);			
			int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);	
			int i = 0;		
			for(; i < FD_SETSIZE; i++)
			{
				if(client[i] < 0)	
				{
					client[i] = connfd;
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				printf("Khong the xu ly them client!!!\n");
				char* connectError = "Khong the ket noi toi server!!!\n";
				writen(client[i], connectError, strlen(connectError));
				continue;
			}
			else 
			{
				char* welcome = "Gui lenh theo cu phap: \"client_id: <ten>\"\n";
				writen(client[i], welcome, strlen(welcome));
			}
			if (maxi < i) maxi = i;
			if (maxfd < connfd) maxfd = connfd;			
		}

		for(int i = 0; i <= maxi; i++)
		{
			if(client[i] < 0) continue;
			if(FD_ISSET(client[i], &fd_read))
			{
				char buffer[1024];
				if (readline(client[i], buffer, sizeof(buffer)) == 0) // neu nhan duoc EOF
				{
					close(client[i]);	//dong socket
					client_status[i] = 0;	//reset trang thai dang nhap client
					client[i] = -1;		//xoa descriptor trong mang client
					free(name[i]);		//xoa ten
					printf("1 client da thoat!!!\n");
					continue;
				}			
				if(client_status[i] == 0) //chua dang nhap vao room chat
				{
					if(strncmp(buffer, "client_id: ", 11) == 0)
					{
						char* valid = "CHUC MUNG BAN DA VAO ROOM\n";
						writen(client[i], valid, strlen(valid));
						client_status[i] = 1; //da vao room
						name[i] = (char*)calloc(MAXLINE, sizeof(char));	
						strcpy(name[i], buffer + 11);
						name[i][strlen(name[i]) - 1] = 0;
					}
					else
					{
						char* invalid = "SAI CU PHAP\n";
						writen(client[i], invalid, strlen(invalid));
					}
				}
				else //da dang nhap vao room chat
				{
					char message[1024];
					sprintf(message, "%s: %s", name[i], buffer);
					for(int k = 0; k <= maxi; k++)
					{
						if(client_status[k] == 1 && k != i)
						{
							writen(client[k], message, strlen(message));	
						}
					}
				}
			}
		}		
	}
	return 0;
}
