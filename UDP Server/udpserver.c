#include "mylib.h"


void send_file(int sockfd, struct sockaddr* cliaddr, int clen, char* path)
{
	char buffer[1024];
	FILE *fi = fopen(path, "rt");	
	int n;		
	while( (n = read(fileno(fi), buffer, sizeof(buffer))) > 0)
	{
		sendto(sockfd, buffer, n, 0, cliaddr, clen);
	}
	fclose(fi);
	char* eof = "EOF";
	sendto(sockfd, eof, strlen(eof), 0, cliaddr, clen);
	printf("Done!!!\n");
}


int main(int argc, char** argv)
{
	int listenfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	char buffer[1024];
	char path[1024];
	memset(buffer, 0, sizeof(buffer));
	
	while(1)
	{
		struct sockaddr_in cliaddr;
		int clen = sizeof(cliaddr);
		recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &clen);
		buffer[strlen(buffer) - 1] = 0;		
		if(strncmp(buffer, "give me ", 8) == 0)
		{
			strcpy(path, buffer + 8);
			char* valid = "OK";
			sendto(listenfd, valid, strlen(valid), 0, (struct sockaddr*)&cliaddr, clen);
			send_file(listenfd, (struct sockaddr*)&cliaddr, clen, path);
		}
		else
		{
			char* invalid = "ERROR";
			sendto(listenfd, invalid, strlen(invalid), 0, (struct sockaddr*)&cliaddr, clen);
		}
		
	}
	return 0;
}
