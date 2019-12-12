#include "defs.h"

// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Ancelmo Polanco

int shmID;
int msgID;
int babyPid;
st_sharedMem* shareMem;


int isPerfectNum(int val){
    if (val < 2) return 0;

    int sum = 1;

    for (int i = 2; i < val; i++){
        int temp = val % i;
        if(!(temp)){
            sum = sum + i;
        }
    }

    
    if (sum == val) return 1;
    else return 0;

}

void endAll(int sigNum){

    if(babyPid == -1) exit(0);

    shareMem->process[babyPid].pid = 0;
    shareMem->process[babyPid].numPerfect = 0;
    shareMem->process[babyPid].numNotTested = 0;
    memset(&shareMem->process[babyPid],0,sizeof(st_process));
}

int main(int argc, char* argv[]){


    int read;

    if (argc != 2) read = 2;
    else atoi(argv[1]);

    shmID = shmget(memKey,sizeof(st_sharedMem),0666);
    if(shmID == -1) exit(-1);

    shareMem = shmat(shmID,NULL,0);

    struct sigaction action;
    action.sa_handler = endAll;

    while(1){
        if( (sigaction(SIGINT,&action,NULL)) == -1 ) exit(1);
        if( (sigaction(SIGQUIT,&action,NULL)) == -1) exit(1);
        if( (sigaction(SIGHUP,&action,NULL)) == -1) exit(1);
        break;
    }


    msgID = msgget(mesKey,0666);

}