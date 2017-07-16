#include "mylib.h"

int main(int argc, char **argv)
{
	my_array myArray;
	printf("The number of the elements: ");
	scanf("%d", &myArray.n);
	for(int i = 0; i < myArray.n; i++)
	{
		printf("Element %d: ", i + 1);
		scanf("%d", &myArray.a[i]);
	}
	int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	unlink(UNIXSTR_PATH);
	struct sockaddr_un servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);;
	
	write(connfd, &myArray, sizeof(myArray));
	read(connfd, &myArray, sizeof(myArray));

	printf("After sorting: \n");
	for(int i = 0; i < myArray.n; i++)
	{
		printf("%5d ", myArray.a[i]);
	}
	printf("\n");
	close(connfd);
	close(listenfd);
	return 0;
}
