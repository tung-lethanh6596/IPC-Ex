#include "mylib.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

void err_sys(const char* s) 
{ 
    perror(s); 
    exit(1); 
}

void recvSendClient(int sockfd)
{
	int val = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, val | O_NONBLOCK);
	
	val = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

	val = fcntl(STDOUT_FILENO, F_GETFL, 0);
	fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);
	
	char to[MAXLINE], fr[MAXLINE];
	char* toiptr, *tooptr, *friptr, *froptr;
	
	toiptr = tooptr = to;
	friptr = froptr = fr;
	int stdineof = 0;
	
	int maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
	fd_set rset, wset;
	int n;
	int nwritten;

	while(1)
	{
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(stdineof == 0 && toiptr < &to[MAXLINE])
		{
			FD_SET(STDIN_FILENO, &rset);
		}
		if(friptr < &fr[MAXLINE])
		{
			FD_SET(sockfd, &rset);
		}
		if(tooptr != toiptr)
		{
			FD_SET(sockfd, &wset);
		}
		if(froptr != friptr)
		{
			FD_SET(STDOUT_FILENO, &wset);
		}
		select(maxfdp1, &rset, &wset, NULL, NULL);
		
		if(FD_ISSET(STDIN_FILENO, &rset))	
		{
			if( (n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0)
			{
				if(errno != EWOULDBLOCK)
					err_sys("read error on stdin");
			}
			else if(n == 0)
			{
				stdineof = 1;
				if(tooptr == toiptr) 
				{
					shutdown(sockfd, SHUT_WR);
				}
			}
			else
			{
				toiptr += n;
				FD_SET(sockfd, &wset);
			}
		}
		
		if(FD_ISSET(sockfd, &rset))
		{
			if( (n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0)
			{
				if(errno != EWOULDBLOCK)
					err_sys("read error on stdin");
			}
			else if(n == 0)
			{
				if(stdineof) return;
				else 
				{
					printf("problem: server terminated prematurely\n");
					return;
				}
			}
			else
			{
				friptr += n;
				FD_SET(STDOUT_FILENO, &wset);
			}
		}
		
		if(FD_ISSET(STDOUT_FILENO, &wset) && ((n = friptr - froptr) > 0))
		{
			if( (nwritten = write(STDOUT_FILENO, froptr, n)) < 0)
			{
				if(errno != EWOULDBLOCK)
				{
					err_sys("write error to stdout");
				}
			} else 
			{
				froptr += nwritten;
				if(froptr == friptr) froptr = friptr = fr;			
			}
		}
		
		if(FD_ISSET(sockfd, &wset) && ((n = toiptr - tooptr) > 0))
		{
			if( (nwritten = write(sockfd, tooptr, n)) < 0)
			{
				if(errno != EWOULDBLOCK)
				{
					err_sys("write error to socket");
				}
			} else
			{
				tooptr += nwritten;
				if(tooptr == toiptr) tooptr = toiptr = to;
				if(stdineof) shutdown(sockfd, SHUT_WR);
			}
		}
	}
}

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("Syntax: command <IP_Address> <Port_Numer>");
		return -1;
	}
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	recvSendClient(sockfd);
	return 0;
}
