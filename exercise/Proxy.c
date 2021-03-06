#include "mylib.h"

#define MAX_CLIENT 100
#define MAX_LENGTH_PACKET 1030

int cnum = 0;
int listenfd;
int recv_fd_1 = 0;
int recv_fd_2 = 0;

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

int change_and_send_packet(packet_t* pkt, int type)
{
	char temp[1024];
	int len = 0;
	switch(type)
	{
		case 0x02:
			memset(temp, 0, sizeof(temp));
			char *c = pkt->value;
			while(*c)
			{
				temp[len++] = *c;
				c += 1;
			}
			break;
		case 0x04:
			break;
		default: 
			return -1;
	}
	
	packet_t* new_packet = calloc(sizeof(packet_t) + len, 1);

	new_packet->header = pkt->header;
	new_packet->type = pkt->type;
	new_packet->length = htons(len);
	if(len)
	{
		strcpy(new_packet->value, temp);
		printf("Send packet: %s\n", new_packet->value);
	}
	
	if(ntohs(new_packet->header) % 2 == 0)
	{
		if(recv_fd_1)
			write(recv_fd_1, new_packet, sizeof(packet_t) + len);
	}
	else
	{
		if(recv_fd_2)
			write(recv_fd_2, new_packet, sizeof(packet_t) + len);
	}
	return 0;
}

void* client_process(void *arg)
{
	pthread_detach(pthread_self());
	int sockfd = *((int*)arg);

	pthread_mutex_lock(&client_mutex);
	cnum += 1;
	pthread_mutex_unlock(&client_mutex);

	char buffer[MAX_LENGTH_PACKET];
	int n;

	while(1)
	{
		memset(buffer, 0, sizeof(buffer));
		n = readn(sockfd, buffer, sizeof(buffer));
		if(n == 0) return NULL;
		packet_t *received_packet = (packet_t*)buffer;
		switch(received_packet->type)
		{
			case 0x04:
				close(sockfd);
				printf("Server has just disconnected a client...\n");
				change_and_send_packet(received_packet, 0x04);
				pthread_mutex_lock(&client_mutex);
				cnum -= 1;
				if(cnum == 0) 
				{
					if(recv_fd_1)
						fill_and_send_packet(NULL, 0x00, 0x04, 0, recv_fd_1);
					if(recv_fd_2)
						fill_and_send_packet(NULL, 0x00, 0x04, 0, recv_fd_2);
				}
				pthread_mutex_unlock(&client_mutex);
				break;
			case 0x02:
				printf("Proxy received a packet, ready to forward...\n");
				change_and_send_packet(received_packet, 0x02);
				break;
		}
	}
	close(sockfd);
	return NULL;
}

int main(int argc, char** argv)
{

	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PROXY_PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	printf("Start proxy server...\n");

	while(1)
	{
		struct sockaddr_in cliaddr;
		socklen_t clen = sizeof(cliaddr);
		int* connfd = (int*)calloc(1, sizeof(int));		
		*connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clen);
		if(ntohs(cliaddr.sin_port) == CLIENT_1_PORT)
		{
			recv_fd_1 = *connfd;
			printf("Receiver 1 connected\n");
			continue;
		}
		if(ntohs(cliaddr.sin_port) == CLIENT_2_PORT)
		{
			recv_fd_2 = *connfd;
			printf("Receiver 2 connected\n");
			continue;
		}
		pthread_t tid;
		pthread_create(&tid, NULL, &client_process, connfd);		
	}
	close(listenfd);
	return 0;
}
