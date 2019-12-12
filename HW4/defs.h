#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>

#define memKey 87010
#define mesKey 65341

// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Ancelmo Polanco


typedef struct {
    pid_t pid;
    int numPerfect;
    int numTested;
    int numNotTested;
} st_process;


typedef struct {
    int num[1050000];
    int perfectNum[20];
    st_process process[20];
    int managePid;
} st_sharedMem;

typedef struct{
    long type;
    int cont;
} st_message;



