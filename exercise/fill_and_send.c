#include "mylib.h"

int fill_and_send_packet(char* value, int myNum, int type, int length, int sockfd)
{
	packet_t* my_packet = calloc(sizeof(packet_t) + length, 1);
	bzero(my_packet, sizeof(packet_t) + length);
	my_packet->header = htons(myNum);
	my_packet->type = type;
	my_packet->length = htons(length);
	if(length)
		strcpy(my_packet->value, value);

	int n = writen(sockfd, my_packet, sizeof(packet_t) + length);
	printf("Sent %d bytes\n", n);
	return 0;
}
