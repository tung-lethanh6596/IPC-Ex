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
	
	int mqid = msgget(MQ_KEY, IPC_CREAT | 0666);
	my_array myArray;
	
	printf("The number of the elements: ");
	scanf("%d", &myArray.n);

	for(int i = 0; i < myArray.n; i++)
	{
		printf("Phan tu thu %d: ", i + 1);
		scanf("%d", &myArray.arrInt[i]);
	}
	msgsnd(mqid, &myArray, (MAX_NUM+1)*sizeof(int), 0);
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = 1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);
	
	sop.sem_num = 1;
	sop.sem_op = -1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);
	msgrcv(mqid, &myArray, (MAX_NUM+1)*sizeof(int), 0, 0);
	
	printf("Day sau khi da sap xep: \n");
	for(int i = 0; i < myArray.n; i++)
	{
		printf("%5d ", myArray.arrInt[i]);
	}
	printf("\n");

	semctl(semid, 0, IPC_RMID, arg);
	semctl(semid, 0, IPC_RMID, arg);
	msgctl(mqid, IPC_RMID, NULL);
	return 0;
}
