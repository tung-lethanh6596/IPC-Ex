#include "mylib.h"

char** sorting(char** str, int size)
{
	int i, j;
	char* temp = NULL;
	for(i = 1; i < size; i++)
		for(j = size - 1; j >= i; j--)
				if(strlen(str[j]) < strlen(str[j - 1]))
				{
					temp = str[j];
					str[j] = str[j - 1];
					str[j - 1] = temp;
				}
	return str;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("command ./Receiver <number>!!!");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in cliaddr;
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	if(atoi(argv[1]) == 1)
		cliaddr.sin_port = htons(CLIENT_1_PORT);
	else 
		cliaddr.sin_port = htons(CLIENT_2_PORT);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PROXY_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	printf("Receiver has connected!!!\n");

	char buffer[1030];
	int n, i;
	packet_t *my_packet = NULL;
	char **recved_packet = (char**)calloc(MAX_SIZE, sizeof(char*));
	int count = 0;
	
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		n = read(sockfd, buffer, sizeof(buffer));
		printf("Received %d bytes\n", n);
		if(n == 0) break;
		my_packet = (packet_t*)buffer;
		switch(my_packet->type)
		{
			case 0x02:
				printf("Recved: %s\n", my_packet->value);
				recved_packet[count] = (char*)calloc(strlen(my_packet->value), 1);
				bzero(recved_packet[count], strlen(my_packet->value));
				memcpy(recved_packet[count], my_packet->value, strlen(my_packet->value));
				count += 1;
				break;
			case 0x04:
				printf("Received end packet\n");
				if(count > 0)
				{
					FILE *fo = NULL;
					switch(atoi(argv[1]))
					{
						case 1:
							fo = fopen(FILE_OUT_1, "wb");
							break;
						case 2:
							fo = fopen(FILE_OUT_2, "wb");
							break;
					}	
					recved_packet = sorting(recved_packet, count);
					for(i = 0; i < count; i++)
						fwrite(recved_packet[i], 1, strlen(recved_packet[i]), fo);
					fclose(fo);
					printf("Saved file!!!");
				}
				count = 0;
				break;
		}
	}	
	return 0;
}
