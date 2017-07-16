#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <sys/un.h>

#define MAXLINE 1024
#define UNIXSTR_PATH "/tmp/unix.str"

typedef struct
{
	int n;
	int a[100];
} my_array;
