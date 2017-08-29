#include "mylib.h"

int myNum;
int sockfd;

int read_file(char* fileName);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Error, command: ./Sender <arg>");
		return 1;
	}
	myNum = atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PROXY_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	printf("Connected to proxy server\n");

	read_file(FILE_IN);

	printf("End of sender %d...\n", myNum);
	close(sockfd);
	return 0;
}

int read_file(char* fileName)
{
	FILE *fi = fopen(fileName, "rb");
	char buffer[1024];
	int i = 0;
	
	memset(buffer, 0, sizeof(buffer));
	while(readn(fileno(fi), buffer, 1024) > 0)
	{
		if((i - myNum) % 16 == 0)
		{
			printf("Send packet type 0x02\n");
			fill_and_send_packet(buffer, myNum, 0x02, 1024, sockfd);
		}
		i += 1;
		usleep(20000);
		memset(buffer, 0, sizeof(buffer));
	} 
	printf("Send packet type 0x04\n");
	fill_and_send_packet(NULL, myNum, 0x04, 0, sockfd);
	fclose(fi);

	return 0;
}

