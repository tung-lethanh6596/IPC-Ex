#include <stdio.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <stdlib.h>

#define MQ_KEY 2000
#define SEM_KEY 1996
#define MAX_NUM 100

union semun {
        int val;    
        struct semid_ds *buf;    
        unsigned short  *array;  
	struct seminfo  *__buf;  
};

typedef struct
{
	long mtype;
	int n;
	int arrInt[MAX_NUM];
} my_array;
