#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void sortProcess(int pcchild1[2], int pcchild2[2]){

    int fd = fdopen(pcchild1[1],"w");

    dup2(pcchild1[0],STDIN_FILENO);
    dup2(pcchild2[1],STDOUT_FILENO);

    



}

void createProcesses(){

    int pchild1[2];
    int pchild2[2];

    pipe(pchild1);
    pipe(pchild2);

    __pid_t pidCh1, pidCh2;



    char * buffer;
    int c;

    FILE * fd;
    int count = 0;


    switch (pidCh1 = fork()){
        case -1:
            printf("Error");
        case 0:
            //sortProcess( pchild1,pchild2);
            dup2(pchild1[0],STDIN_FILENO);
            c = fgetc(pchild1[0]);
            close(pchild1[0]);
            close(pchild1[1]);
        ;


            // dup2(pchild2[1],STDOUT_FILENO);

        default:
            // c = fgetc(stdin);
            // printf("%c",c);
            
            fd = fdopen(pchild1[1],"w");
            do{
                c = fgetc(stdin);


                if(isalpha(c) == 0){
                    fputc('\n',fd);
                    count = 0;
                }
                else if (isalpha(c) != 0 && count <= 32){
                    fputc(c,fd);
                    count ++;
                }


                if(feof(stdin)) break;
                 // printf("%c",c);

            }while(1);
            
            waitpid(pidCh1,NULL,0);
            close(pchild1[0]);
            close(pchild1[1]);
            //execl bin/sort
    }
}

int main(int argc, char* argv[]){
    createProcesses();
}