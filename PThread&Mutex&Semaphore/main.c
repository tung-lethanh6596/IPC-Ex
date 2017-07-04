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

int num_threads_done;
sem_t building_array;
sem_t finished_building_array;
sem_t finished_arranging_array;

void* readData(void *);		//each thread read data file
void* arrangeWithName(void *);	//a thread arranges students and outputs to file arrangeWithName.txt
void* countOn(void *);		//a thread used to sum up all students and output to file countOn.txt

//Additional functions for sorting
void swapStudent(student* a, student* b);
char* findFirstName(char* s);

int main(int argc, char** argv)
{
	char path[100];
	pthread_t tid[12];
	FILE* fi[10];
	
	if (sem_init(&building_array, 0, 1)) 		//initially unlocked
    	{
      	 	perror("Semaphore initialization failed");
       		exit(0);
    	}
	
	if (sem_init(&finished_building_array, 0, 0)) 	//initially locked
    	{
        	perror("Semaphore initialization failed");
        	exit(0);
    	}
	
	if (sem_init(&finished_arranging_array, 0, 0)) 	//initially locked
    	{
        	perror("Semaphore initialization failed");
        	exit(0);
    	}

	num_threads_done = 0;		

	for(int i = 0; i < 10; i++)
	{
		strcpy(path, "data/");
		path[5] = i + 48;
		path[6] = 0;
		fi[i] = fopen(path, "rt");
		pthread_create(&tid[i], NULL, readData, (void*)fi[i]);	
	}
	
	pthread_create(&tid[10], NULL, arrangeWithName, NULL);	
	pthread_create(&tid[11], NULL, countOn, NULL);
	
	for(int i = 0; i < 12; i++)	//wait for all threads done
	{
		pthread_join(tid[i], NULL);	
	}

	pthread_mutex_destroy(&array_mutex);	
	sem_destroy(&building_array);
    	sem_destroy(&finished_building_array);
	
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

	sem_wait(&building_array);
	if(num_threads_done == 9)
	{
		sem_post(&finished_building_array);
	}
	else
	{
		num_threads_done++;
		sem_post(&building_array);
	}
	return NULL;
}

void* arrangeWithName(void *arg)
{
	sem_wait(&finished_building_array);
	int res;
	for(int i = 0; i < stNum; i++)
	{
		for(int j = stNum - 1; j > i; j--)
		{
			char tmp[100];
			strcpy(tmp, findFirstName(students[j].name));
			if( (res = strcmp(tmp, findFirstName(students[j-1].name))) < 0)
			{
				swapStudent(&students[j], &students[j-1]);
			}
			else if(res == 0)
			{
				if(strcmp(students[j].name, students[j-1].name) < 0)
				{
					swapStudent(&students[j], &students[j-1]);
				}
			}	
		}
	}
	FILE* fo = fopen("arrangeWithName.txt", "wt");
	for(int i = 0; i < stNum; i++)
	{
		fprintf(fo, "%d %s\n", students[i].stnumber, students[i].name);
	}
	fclose(fo);
	sem_post(&finished_arranging_array);
	return NULL;
}

char* findFirstName(char* s)
{
	int i = strlen(s) - 1;
	while(!isspace(s[i])) i--;
	return &s[i+1];
}

void swapStudent(student* a, student* b)
{
	int tmp;
	tmp = a->stnumber;
	a->stnumber = b->stnumber;
	b->stnumber = tmp;
	char tmp_name[100];
	strcpy(tmp_name, a->name);
	strcpy(a->name, b->name);
	strcpy(b->name, tmp_name);
}

void* countOn(void *arg)
{
	sem_wait(&finished_arranging_array);
	int res[1000];
	memset(res, 0, sizeof(res));
	FILE* fo = fopen("countOn.txt", "wt");
	for(int i = 0; i < stNum; i++)
	{
		res[(students[i].stnumber / 10000) % 1000]++;		
	}
	for(int i = 0; i < 1000; i++)
	{
		if(res[i] != 0)
		{
			fprintf(fo, "%d: %d students\n", 2000 + i, res[i]);
		}
	}
	fclose(fo);
	return NULL;
}	

