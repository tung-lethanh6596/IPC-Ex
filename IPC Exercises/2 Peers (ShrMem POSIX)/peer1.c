#include "libs.h"

int main(int argc, char** argv)
{
	int shmfd = shm_open(SHMNAME, O_CREAT | O_RDWR, 0666);
	ftruncate(shmfd, sizeof(my_array));
	my_array* myArray = mmap(NULL, sizeof(my_array), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	
	sem_t *sem_finished_entering = sem_open(SEM_FINISHED_ENTERING, O_CREAT, 0666, 0);
	sem_t *sem_finished_arranging = sem_open(SEM_FINISHED_ARRANGING, O_CREAT, 0666, 0);

	printf("The number of the elements: ");
	scanf("%d", &(myArray->n));
	for(int i = 0; i < myArray->n; i++)
	{
		printf("Element %d: ", i + 1);
		scanf("%d", &(myArray->arrInt[i]));
	}
	sem_post(sem_finished_entering);

	sem_wait(sem_finished_arranging);
	
	printf("\nArray after sorting: \n");	
	for(int i = 0; i < myArray->n; i++)
	{
		printf("%5d ", myArray->arrInt[i]);
	}
	printf("\n");
	munmap(SHMNAME, sizeof(my_array));
	sem_close(sem_finished_entering);
	sem_close(sem_finished_arranging);
	sem_unlink(SEM_FINISHED_ENTERING);
	shm_unlink(SHMNAME);
	return 0;
}
