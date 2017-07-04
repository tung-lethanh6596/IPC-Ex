#include "mylib.h"

void send_file(int connfd, char* file_path)
{
	
	FILE* fi = fopen(file_path, "rb");
	char buffer[1024];
	int n;
	while((n = readn(fileno(fi), buffer, MAXLINE)) > 0)
	{
		writen(connfd, buffer, n);
	} 
	fclose(fi);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("command <file_name>!!!");
		return -1;
	}
	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	pid_t childpid;

	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	while(1)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
		printf("A client has connected: [%s:%d]\n", inet_ntoa(cliaddr.sin_addr), 
								ntohs(cliaddr.sin_port));
		if((childpid = fork()) == 0)
		{
			close(listenfd);
			send_file(connfd, argv[1]);
			printf("Completed sending to [%s:%d]\n", inet_ntoa(cliaddr.sin_addr), 
								ntohs(cliaddr.sin_port)); 		
			exit(0);
		}
		close(connfd);			
	}	
	return 0;
}
