#include "mylib.h"


void client_process(FILE* fp, int sockfd)
{
	fd_set fdread;
	char buffer[1024];
	int n;	
	int stdineof = 0;
	int maxfdp1 = ( (sockfd > STDIN_FILENO) ? sockfd : STDIN_FILENO) + 1;	
	FD_ZERO(&fdread);
	while(1)
	{	
		if(stdineof == 0)
			FD_SET(STDIN_FILENO, &fdread);
		FD_SET(sockfd, &fdread);		
		select(maxfdp1, &fdread, NULL, NULL, NULL);
		if(FD_ISSET(sockfd, &fdread))
		{
			if ( (n = read(sockfd, buffer, sizeof(buffer))) == 0)
			{
				if(stdineof) return;
				else 
				{
					printf("Server terminated prematurely!\n");
					exit(0);
				}
			}
			write(fileno(stdout), buffer, n);
		}

		if(FD_ISSET(fileno(fp), &fdread))
		{
			if( (n = read(fileno(fp), buffer, sizeof(buffer))) == 0)
			{
				stdineof = 1;
				shutdown(sockfd, SHUT_WR);
				FD_CLR(STDIN_FILENO, &fdread);
				continue;
			}
			writen(sockfd, buffer, n);
		}
	}
}


int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Syntax: command <IP address> <Port>\n");
		return -1;
	}
	
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	client_process(stdin, sockfd);	
	
	return 0;	
}
