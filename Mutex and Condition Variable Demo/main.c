#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
	int stnumber;
	char name[100];
} student;

student students[1000];
int stNum = 0;

pthread_mutex_t array_mutex = PTHREAD_MUTEX_INITIALIZER;

int ndone = 0;
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ndone_cond = PTHREAD_COND_INITIALIZER;

void* readData(void* arg);

int main(int argc, char** argv)
{
	char path[100];
	pthread_t tid[12];
	FILE* fi[10];

	for(int i = 0; i < 10; i++)
	{
		strcpy(path, "data/");
		path[5] = i + 48;
		path[6] = 0;
		fi[i] = fopen(path, "rt");
		pthread_create(&tid[i], NULL, readData, (void*)fi[i]);	
	}
	
	pthread_cond_wait(&ndone_cond, &ndone_mutex);
	
	printf("Read all file, total: %d students\n", stNum);
	pthread_mutex_destroy(&array_mutex);	
	pthread_mutex_destroy(&ndone_mutex);
	
	return 0;
}

void* readData(void* arg)
{
	FILE* fi = (FILE*)arg;
	int stnumber;
	char line[100];
	char res[100];
	while(fgets(line, sizeof(line), fi) != NULL)
	{
		strncpy(res, line, 8);
		stnumber = atoi(res);
		strcpy(res, line + 9);
		res[strlen(res)-1] = 0;
		pthread_mutex_lock(&array_mutex);		
		students[stNum].stnumber = stnumber;
		strcpy(students[stNum].name, res);
		stNum += 1;
		pthread_mutex_unlock(&array_mutex);
	}
	fclose(fi);

	pthread_mutex_lock(&ndone_mutex);
	ndone += 1;
	if(ndone == 10)
	{
		pthread_cond_signal(&ndone_cond);
	}	
	pthread_mutex_unlock(&ndone_mutex);
	return NULL;
}




