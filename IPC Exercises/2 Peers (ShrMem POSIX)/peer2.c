#include "libs.h"

int main(int argc, char** argv)
{
	int shmfd = shm_open(SHMNAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shmfd, sizeof(my_array));
	my_array* myArray = mmap(NULL, sizeof(my_array), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	
	sem_t *sem_finished_entering = sem_open(SEM_FINISHED_ENTERING, O_CREAT, 0666, 0);
	sem_t *sem_finished_arranging = sem_open(SEM_FINISHED_ARRANGING, O_CREAT, 0666, 0);
	
	sem_wait(sem_finished_entering);
		
	for(int i = 0; i < myArray->n; i++)
	{
		for(int j = myArray->n - 1; j > i; j--)
			if(myArray->arrInt[j] < myArray->arrInt[j-1])
			{
				int temp = myArray->arrInt[j];
				myArray->arrInt[j] = myArray->arrInt[j-1];
				myArray->arrInt[j-1] = temp;
			}
	}

	sem_post(sem_finished_arranging);
	
	FILE* fo = fopen("res.txt", "wt");
	for(int i = 0; i < myArray->n; i++)
	{
		fprintf(fo, "%5d ", myArray->arrInt[i]);
	}
	fclose(fo);
	printf("Finished writing to file!!!\n");
	munmap(SHMNAME, sizeof(my_array));
	sem_close(sem_finished_entering);
	sem_close(sem_finished_arranging);
	sem_unlink(SEM_FINISHED_ARRANGING);
	
	return 0;
}
