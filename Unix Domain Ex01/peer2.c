#include "mylib.h"

int main(int argc, char **argv)
{
	my_array myArray;
	int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	struct sockaddr_un servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	read(sockfd, &myArray, sizeof(myArray));
	for(int i = 0; i < myArray.n; i++)
		for(int j = myArray.n - 1; j > i; j--)
			if(myArray.a[j] < myArray.a[j-1])
			{
				int tmp = myArray.a[j];
				myArray.a[j] = myArray.a[j-1];
				myArray.a[j-1] = tmp;
			}
	write(sockfd, &myArray, sizeof(myArray));
	
	close(sockfd);
	return 0;
}
