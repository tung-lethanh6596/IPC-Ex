#include "libs.h"

int main(int argc, char** argv)
{	
	mqd_t mqd = mq_open(MQ_NAME, O_RDWR);
	my_array myArray;
	ssize_t n = mq_receive(mqd, (char*)&myArray, sizeof(myArray), NULL);
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
	mq_send(mqd, (const char*)&myArray, sizeof(myArray), 0);
	
	FILE* fo = fopen("res.txt", "wt");
	for(int i = 0; i < myArray.n; i++)
	{
		fprintf(fo, "%5d ", myArray.arrInt[i]);
	}
	fclose(fo);
	printf("Finished writing to file!!!\n");
	
	mq_close(mqd);
	
	return 0;
}
