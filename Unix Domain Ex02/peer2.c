#include "unp.h"

int main(int argc, char **arv)
{
	int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	struct sockaddr_un servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);
	
	int e = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	char c;
	int fd;
	read_fd(sockfd, &c, 1, &fd);
	
	my_array myArray;
	char buffer[1024];
	readline(fd, buffer, sizeof(buffer));
	sscanf(buffer, "%d", &myArray.n);
	readline(fd, buffer, sizeof(buffer));
	buffer[strlen(buffer) - 1] = ' ';
	for(int i = 0; i < myArray.n; i++)
	{
		sscanf(buffer, "%d", &myArray.a[i]);
		strcpy(buffer, strchr(buffer, ' ') + 1);
	}

	write(sockfd, &myArray, sizeof(myArray));
	printf("Done!!!\n");
	close(sockfd);
	return 0;
}
