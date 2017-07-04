#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/errno.h>
#include <stdlib.h>

#define SHM_KEY 1234
#define SEM_KEY 2004

union semun {
        int val;    
        struct semid_ds *buf;    
        unsigned short  *array;  
	struct seminfo  *__buf;  
};

typedef struct
{
	int n;
	int arrInt[100];
} my_array;
