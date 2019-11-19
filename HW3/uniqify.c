#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING

// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Ancelmo Polanco

void createProcesses(){

    int pchild1[2];
    int pchild2[2];

    pipe(pchild1);
    pipe(pchild2);

    __pid_t pidCh1, pidCh2;



    char * buffer;
    int c;

    FILE * fd;
    FILE * fd1;
    int count = 0;
    char word[34];
    

    switch (pidCh1 = fork()){
        case -1:
            printf("Error");
        case 0:
            dup2(pchild1[0],STDIN_FILENO);
            dup2(pchild2[1],STDOUT_FILENO);
            close(pchild1[1]);
            close(pchild2[0]);
                 
            execl("/usr/bin/sort","sort",(char*) NULL);
            

        default:
            
            fd = fdopen(pchild1[1],"w");
            do{
                c = fgetc(stdin);


                if(isalpha(c) == 0){
                    word[count] = '\n';
                    word[count+1] = '\0';
                    if(count >= 5){ 
                        fputs(word,fd);
                    }

                    count = 0;
                }
                else if (isalpha(c) != 0 && count < 32){
                    word[count] = tolower(c);
                    count ++;
                }


                if(feof(stdin)) break;

            }while(1);

            close(pchild1[0]);
            fclose(fd);
            switch (pidCh2 = fork()){
                case -1:
                    printf("error");
                case 0:
                    dup2(pchild2[0],STDIN_FILENO);
                    close(pchild2[1]);


                    fd1 = fdopen(pchild2[0],"r");
                    char buf[34];

                    char previous[34];
                    int wordCount = 1;
                    int didPrint = 0;
                        
                    if(fgets(buf,34,fd1) != (char *) NULL){
                        strcpy(previous,buf);
                    }

                    while (fgets(buf,34,fd1) != (char *) NULL){
                        if(strcmp(previous,buf) == 0){
                            wordCount ++;
                            didPrint = 1;
                        }
                        else{
                            printf("%5d %s",wordCount,previous);
                            strcpy(previous,buf);
                            wordCount = 1;
                            didPrint = 0;
                        }
                    }
                    if(didPrint == 1){
                        printf("%5d %s",wordCount,buf);
                    }
                    fflush(stdout);
                    fclose(fd1);

                default:
                    close(pchild2[1]);
                    close(pchild2[0]);
                    waitpid(pidCh1,NULL,0); 
                    waitpid(pidCh2,NULL,0); 
            }
                      

    }



}

int main(int argc, char* argv[]){
    createProcesses();
}