#include "libs.h"

int main(int argc, char** argv)
{
	union semun arg;
	int semid = semget(SEM_KEY, 2, IPC_CREAT | IPC_EXCL | 0666);
	/*
		semaphore 0: increase when entering data done.
		semaphore 1: increase when arranging done. 
	*/
	if(semid == -1)
	{
		if(errno != EEXIST) return -1;
		semid = semget(SEM_KEY, 2, 0);
	}
	else
	{
		arg.buf = calloc(2, sizeof(arg.array[0]));
		arg.array[0] = arg.array[1] = 0;	// initially 0 for 2 semaphores	
		semctl(semid, 0, SETALL, arg);
	}
	
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);

	int mqid = msgget(MQ_KEY, 0);

	my_array myArray;
	msgrcv(mqid, &myArray, (MAX_NUM+1)*sizeof(int), 0, 0);
	int n = myArray.n;
	for(int i = 0; i < n; i++)
	{
		for(int j = n - 1; j > i; j--)
			if(myArray.arrInt[j] < myArray.arrInt[j-1])
			{
				int temp = myArray.arrInt[j];
				myArray.arrInt[j] = myArray.arrInt[j-1];
				myArray.arrInt[j-1] = temp;
			}
	}
	msgsnd(mqid, &myArray, (MAX_NUM)+1*sizeof(int), 0);
	sop.sem_num = 1;
	sop.sem_op = 1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);
	
	FILE *fi = fopen("res.txt", "wt");
	for(int i = 0; i < myArray.n; i++)
	{
		fprintf(fi, "%5d ", myArray.arrInt[i]);
	}
	fclose(fi);
	printf("Finished wrting to file!!!\n");
	
	
	return 0;
}
