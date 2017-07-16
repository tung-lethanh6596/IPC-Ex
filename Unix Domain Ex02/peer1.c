#include "unp.h"

int main(int argc, char **arv)
{
	int fd = open("data.txt", O_RDONLY);

	unlink(UNIXSTR_PATH);	
	int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	struct sockaddr_un servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);
	
	int e = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));	
	listen(listenfd, 10);
	
	int connfd;
	struct sockaddr_un cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);	

	write_fd(connfd, "", 1, fd);
	
	my_array myArray;
	read(connfd, &myArray, sizeof(myArray));
	for(int i = 0; i < myArray.n; i++)
	{
		printf("%5d ", myArray.a[i]);
	}
	printf("\n");
	close(connfd);
	close(listenfd);
	return 0;
}
