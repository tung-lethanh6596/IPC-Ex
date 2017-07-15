#include "libs.h"

void handler(int sig)
{
	
}

int main(int argc, char** argv)
{
	struct mq_attr attr;
	attr.mq_flags = 0;
    	attr.mq_maxmsg = 10;
    	attr.mq_msgsize = sizeof(my_array);
    	attr.mq_curmsgs = 0;
	mqd_t mqd = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	
	my_array myArray;
	printf("The number of the elements: ");
	scanf("%d", &myArray.n);
	for(int i = 0; i < myArray.n; i++)
	{
		printf("Element %d: ", i + 1);
		scanf("%d", &myArray.arrInt[i]);
	}
	int error = mq_send(mqd, (const char*)&myArray, sizeof(myArray), 0);
	
	sigset_t blockMask, emptyMask;
	struct sigaction sa;
	
	sigemptyset(&blockMask);
	sigaddset(&blockMask, NOTIFY_SIG);
	sigprocmask(SIG_BLOCK, &blockMask, NULL);
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(NOTIFY_SIG, &sa, NULL);

	struct sigevent sev;
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	mq_notify(mqd, &sev);

	sigemptyset(&emptyMask);
	sigsuspend(&emptyMask);
	
	mq_receive(mqd, (char*)&myArray, sizeof(myArray), NULL);
	printf("\nArray after sorting: \n");	
	for(int i = 0; i < myArray.n; i++)
	{
		printf("%5d ", myArray.arrInt[i]);
	}
	printf("\n");
	mq_close(mqd);
	mq_unlink(MQ_NAME);
	return 0;
}
