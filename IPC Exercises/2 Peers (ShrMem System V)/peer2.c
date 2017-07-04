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
	
	int shmid = shmget(SHM_KEY, 0, 0);
	my_array* myArr = shmat(shmid, NULL, 0);
	if(myArr == (void*) -1)
	{
		printf("Error shmat\n");
		return -1;
	}
	int n = myArr->n;
	for(int i = 0; i < n; i++)
	{
		for(int j = n - 1; j > i; j--)
			if(myArr->arrInt[j] < myArr->arrInt[j-1])
			{
				int temp = myArr->arrInt[j];
				myArr->arrInt[j] = myArr->arrInt[j-1];
				myArr->arrInt[j-1] = temp;
			}
	}

	sop.sem_num = 1;
	sop.sem_op = 1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);

	FILE *fi = fopen("res.txt", "wt");
	for(int i = 0; i < n; i++)
	{
		fprintf(fi, "%5d ", myArr->arrInt[i]);
	}
	fclose(fi);
	printf("Finished wrting to file!!!\n");

	shmdt(myArr);

	return 0;
}
