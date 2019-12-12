#include "defs.h"
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Ancelmo Polanco

int shmId;
int messageId;

st_sharedMem* shared_mem;


void endAll(int sig){
    for (int i = 0; i< 20; i++){
        if (shared_mem->process[i].pid != 0){
            printf("Correct");
            kill(shared_mem->process[i].pid,SIGINT);
        }
    }

    sleep(5);

    shmdt(shared_mem);
    shmctl(shmId,IPC_RMID,NULL);
    msgctl(messageId,IPC_RMID,NULL);

    exit(0);
}

int maint(int argc, char* argv[]){

    shmId = shmget(memKey, sizeof(st_sharedMem),0666 | IPC_CREAT | IPC_EXCL);

    if (shmId == -1) exit(1);

    shared_mem = shmat(shmId,NULL,0); 

    memset(shared_mem, 0 , sizeof(st_sharedMem));
    
    shared_mem->managePid = getpid();

    struct sigaction action;

    action.sa_handler = endAll;



}