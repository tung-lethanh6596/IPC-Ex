#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MQ_NAME "/mymq"
#define SEM_FINISHED_ENTERING "semfnetr"
#define SEM_FINISHED_ARRANGING "semfnarr"

typedef struct 
{
	int n;
	int arrInt[100];
}my_array;
