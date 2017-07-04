#include "mylib.h"

void recv_file(int sockfd, char* storage_path)
{
	FILE* fo = fopen(storage_path, "wb");
	char buffer[1024];
	int n;
	memset(buffer, 0, sizeof(buffer));
	while((n = read(sockfd, buffer, sizeof(buffer))) > 0)
	{
		write(fileno(fo), buffer, n);
	}
	fclose(fo);	
	printf("Done!!!\n");	
}

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	
	if(argc != 4)
	{
		printf("Syntax: command <IP Address> <Port> <received_file>\n");
		return -1;
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	recv_file(sockfd, argv[3]);
	return 0;
}
