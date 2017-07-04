#include "mylib.h"


void recv_file(int sockfd, struct sockaddr* cliaddr, socklen_t clen, char* path)
{
	FILE* fo = fopen(path, "wt");
	int n;
	char buffer[1024];
	//while( (n = recvfrom(sockfd, buffer, sizeof(buffer), 0, cliaddr, &clen)) > 0)
	while( (n = read(sockfd, buffer, sizeof(buffer))) > 0)
	{
		if(strncmp(buffer, "EOF", 3) == 0) break;
		write(fileno(fo), buffer, n);
	} 
	fclose(fo);
	printf("Received!!!\n");
}


int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Syntax: command <IP address> <Port>\n");
		return -1;
	}
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clen;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	char buffer[1024];
	char temp[1024];	
	
	while(fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
		strcpy(temp, buffer);
		write(sockfd, buffer, strlen(buffer));
		//sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
		memset(buffer, 0, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
		//recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &clen);
		printf("%s\n", buffer);
		if(strcmp(buffer, "OK") ==  0)
		{
			char path[1024];
			strcpy(path, "my_download/");
			temp[strlen(temp) - 1] = 0;			
			strcat(path, temp + 8);
			recv_file(sockfd, (struct sockaddr*)&cliaddr, clen, path);
		}
	}
	return 0;
}
