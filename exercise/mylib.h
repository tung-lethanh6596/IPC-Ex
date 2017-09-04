#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 2000

#define FILE_IN "in.bin"

#define FILE_OUT_1 "out1.bin"
#define FILE_OUT_2 "out2.bin"

#define CLIENT_1_PORT 12572
#define CLIENT_2_PORT 12589
#define PROXY_PORT 12594

struct packet
{
	uint16_t header;
	uint8_t type;
	uint8_t padding;
	uint16_t length;
	char value[];
};

typedef struct packet packet_t;

ssize_t readn(int fd, void *vptr, size_t n);

ssize_t writen(int fd, const void *vptr, size_t n);

int fill_and_send_packet(char* value, int myNum, int type, int length, int sockfd);
