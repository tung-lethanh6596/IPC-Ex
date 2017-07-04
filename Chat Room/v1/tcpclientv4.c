#include "mylib.h"

void client_process(int sockfd)
{
	pid_t pid;
	char sendbuff[1024], recvbuff[1024];
	
	if( (pid = fork()) == 0)
	{
		while(readline(sockfd, recvbuff, sizeof(recvbuff)) > 0)
			fputs(recvbuff, stdout);
		kill(getppid(), SIGTERM);
		exit(0);
	}

	while(fgets(sendbuff, sizeof(sendbuff), stdin) != NULL)
		writen(sockfd, sendbuff, strlen(sendbuff));
	shutdown(sockfd, SHUT_WR);
	pause();
	return;
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
	
	client_process(sockfd);		
	return 0;
}
