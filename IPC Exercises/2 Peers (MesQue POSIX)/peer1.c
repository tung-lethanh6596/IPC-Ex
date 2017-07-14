#include "libs.h"

int main(int argc, char** argv)
{
	sem_t *sem_finished_arranging = sem_open(SEM_FINISHED_ARRANGING, O_CREAT, 0666, 0);

	struct mq_attr attr;
	attr.mq_flags = 0;
    	attr.mq_maxmsg = 10;
    	attr.mq_msgsize = sizeof(my_array);
    	attr.mq_curmsgs = 0;
	mqd_t mqid = mq_open(MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	
	my_array myArray;
	printf("The number of the elements: ");
	scanf("%d", &myArray.n);
	for(int i = 0; i < myArray.n; i++)
	{
		printf("Element %d: ", i + 1);
		scanf("%d", &myArray.arrInt[i]);
	}
	int error = mq_send(mqid, (const char*)&myArray, sizeof(myArray), 0);
	
	sem_wait(sem_finished_arranging);
	mq_receive(mqid, (char*)&myArray, sizeof(myArray), NULL);
	printf("\nArray after sorting: \n");	
	for(int i = 0; i < myArray.n; i++)
	{
		printf("%5d ", myArray.arrInt[i]);
	}
	printf("\n");
	mq_close(mqid);
	sem_close(sem_finished_arranging);
	sem_unlink(SEM_FINISHED_ENTERING);
	mq_unlink(MQ_NAME);
	return 0;
}
