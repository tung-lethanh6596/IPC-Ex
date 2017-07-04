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

	int shmid = shmget(SHM_KEY, sizeof(my_array), IPC_CREAT | 0666);
	my_array* myArr = shmat(shmid, NULL, 0);
	if(myArr == (void*) -1)
	{
		printf("Error shmat\n");
		return -1;
	}
	printf("The number of the elements: ");
	scanf("%d", &(myArr->n));
	for(int i = 0; i < myArr->n; i++)
	{
		printf("Element %d: ", i + 1);
		scanf("%d", &(myArr->arrInt[i]));
	}
	struct sembuf sop;
	sop.sem_num = 0;	// increase 1 for semaphore 0, peer2 will run
	sop.sem_op = 1;		
	sop.sem_flg = 0;
	semop(semid, &sop, 1);
	
	sop.sem_num = 1;	// decrease 1 to wait peer2 fisnish arranging
	sop.sem_op = -1;
	sop.sem_flg = 0;
	semop(semid, &sop, 1);
	printf("Array after sorting: ");	
	for(int i = 0; i < myArr->n; i++)
	{
		printf("%5d ", myArr->arrInt[i]);
	}
	printf("\n");
	shmdt(myArr);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 0, IPC_RMID, arg);
	semctl(semid, 0, IPC_RMID, arg);
	return 0;
}
