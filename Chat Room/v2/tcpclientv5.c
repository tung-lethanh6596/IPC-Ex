#include "mylib.h"

int sockfd;


void* sendToServer(void* arg)
{
	pthread_detach(pthread_self());
	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		writen(sockfd, buffer, strlen(buffer));
	}	
	shutdown(sockfd, SHUT_WR);
	return NULL;
}


void client_process() 
{
	pthread_t tid;		
	pthread_create(&tid, NULL, &sendToServer, NULL);
	
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	while(readline(sockfd, buffer, sizeof(buffer)) > 0)
	{
		fputs(buffer, stdout);
		memset(buffer, 0, sizeof(buffer));
	}
}


int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Syntax: command <IP address> <Port>\n");
		return -1;
	}
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	client_process();
}
