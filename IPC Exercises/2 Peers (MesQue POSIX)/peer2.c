#include "libs.h"

int main(int argc, char** argv)
{	
	sem_t *sem_finished_arranging = sem_open(SEM_FINISHED_ARRANGING, O_CREAT, 0666, 0);
	mqd_t mqid = mq_open(MQ_NAME, O_RDWR);
	my_array myArray;
	ssize_t n = mq_receive(mqid, (char*)&myArray, sizeof(myArray), NULL);
	for(int i = 0; i < myArray.n; i++)
	{
		for(int j = myArray.n - 1; j > i; j--)
			if(myArray.arrInt[j] < myArray.arrInt[j-1])
			{
				int temp = myArray.arrInt[j];
				myArray.arrInt[j] = myArray.arrInt[j-1];
				myArray.arrInt[j-1] = temp;
			}	
	}
	mq_send(mqid, (const char*)&myArray, sizeof(myArray), 0);
	sem_post(sem_finished_arranging);
	
	FILE* fo = fopen("res.txt", "wt");
	for(int i = 0; i < myArray.n; i++)
	{
		fprintf(fo, "%5d ", myArray.arrInt[i]);
	}
	fclose(fo);
	printf("Finished writing to file!!!\n");
	
	mq_close(mqid);
	sem_close(sem_finished_arranging);
	sem_unlink(SEM_FINISHED_ARRANGING);
	
	return 0;
}
