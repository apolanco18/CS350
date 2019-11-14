#include <ar.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <utime.h>
#include <errno.h>

//  "This code is my own work, it was written without consulting a tutor or code written by other students - Ancelmo Polanco". 

struct meta {
    char name[16]; //room for null
    int mode;
    int size;
    int uid;
    int gid;
    time_t mtime;
};


int fill_ar_hdr(char *filename, struct ar_hdr *hdr){
    struct stat temp;

    int fd = stat(filename,&temp);

    if(fd == -1){
        return -1;
    }

    char quickFix;
    char quickFix2;

    sprintf(hdr->ar_date,"%-12d",temp.st_mtime);

    quickFix = hdr->ar_date[0];

    sprintf(hdr->ar_size,"%-10d", temp.st_size);

    quickFix2 = hdr->ar_size[0];

    sprintf(hdr->ar_uid,"%-6d", temp.st_uid);

    sprintf(hdr->ar_gid,"%-6d", temp.st_gid);

    sprintf(hdr->ar_mode,"%-8o", temp.st_mode);

    sprintf(hdr->ar_fmag,"%s", ARFMAG);

    

    sprintf(hdr->ar_name,"%-16s",filename);
    hdr->ar_date[0] = quickFix;
    hdr->ar_size[0] = quickFix2;
    hdr->ar_name[strlen(filename)] = '/';

    
    return 0;
}

void option_q(char* archFile, char* filename){
    int fd;
    if( (open(archFile, O_RDWR | O_CREAT | O_EXCL , 0666)) == -1){
        if (errno == EEXIST){
            fd = open(archFile,O_RDWR,0666);
            
        }
    }
    else
    {
        fd = open(archFile,O_RDWR | O_CREAT,0666);
        write(fd,"!<arch>\n",8);
    }
    

    char test[8];
    read(fd,&test,8);


    if(errno == EEXIST){
        if(strcmp(test,"!<arch>\n") != 0){
            printf("Wrong File Format \n");
            return;
        }
    }

    
    struct ar_hdr arHdr;

    fill_ar_hdr(filename, &arHdr);

    lseek(fd,0,SEEK_END);

    write(fd,&arHdr, 60);

    int fd2 = open(filename, O_RDONLY);

    struct stat appFile;

    stat(filename,&appFile);

    int blockSize = (int) appFile.st_blksize;

    int totalSize = (int) appFile.st_size;

    while(totalSize > 0){
        if(totalSize < blockSize){
                char buff[totalSize];

                read(fd2,buff,sizeof(buff));

                lseek(fd,0,SEEK_END);

                write(fd,buff, sizeof(buff));

                totalSize -= totalSize;
        }
        else{
            char buff[blockSize];

            read(fd2,buff,sizeof(buff));

            lseek(fd,0,SEEK_END);

            write(fd,buff, sizeof(buff));

            totalSize -= blockSize;
        }
    }

    close(fd2);


    if( (appFile.st_size % 2) != 0){
        write(fd,"\n",1);
    }


    close(fd);


}

int checkFile(char * filename){
    struct stat temp;
    stat(filename, &temp);
    return S_ISREG(temp.st_mode);
}

void option_A(char* archFile){
    struct dirent* pointDir;

    DIR *dir = opendir(".");

    int fd = open(archFile, O_RDWR);
    if (fd == -1){
        return;
    }

    while((pointDir = readdir(dir)) != NULL){
        if(checkFile(pointDir->d_name)){
            if(strcmp(pointDir->d_name,archFile) == 0 || strcmp(pointDir->d_name,"myar") == 0 || strcmp(pointDir->d_name,"myar.c") == 0){

            }
            else{
                option_q(archFile,pointDir->d_name);
            }
        } 
    }

    close(fd);
    
}

int fill_meta( struct ar_hdr hdr, struct meta * meta){

    char name[sizeof(hdr.ar_name)];
    int mode;
    int size;
    int uid;
    int gid;
    time_t mtime;
    
    memcpy(meta->name,hdr.ar_name,16);
    meta->mode = atoi(hdr.ar_mode);
    meta->size = atoi(hdr.ar_size);
    meta->uid = atoi(hdr.ar_uid);
    meta->gid = atoi(hdr.ar_gid);
    meta->mtime = (time_t) atoi(hdr.ar_date);

    return 0;
}

void option_xo(char* archFile, char* filename){
    int fd = open(archFile, O_RDWR);

    struct stat buffSize;

    stat(archFile,&buffSize);

    if(fd == -1){
        printf("Error Archive File Not Found \n");
        return;
    }

    struct ar_hdr arHdr;
    struct meta tempMeta;

    char test[8];
    read(fd,&test,8);

    if(strcmp(test,"!<arch>\n") != 0){
        printf("Wrong File Format \n");
        return;
    }

    while( (read(fd,&arHdr,60)) != 0 ){
        fill_meta(arHdr,&tempMeta);


        if(strncmp(tempMeta.name,filename,strlen(filename)) == 0){
            

            int fd2 = open(filename,O_RDWR | O_CREAT | O_TRUNC, tempMeta.mode);

            

            int blockSize = (int) buffSize.st_blksize;

            int totalSize = tempMeta.size;

            while(totalSize > 0){
                if(totalSize < blockSize){
                        char buff[totalSize];

                        read(fd,buff,sizeof(buff));

                        write(fd2,buff, sizeof(buff));

                        totalSize -= totalSize;
                }
                else{
                    char buff[blockSize];

                    read(fd,buff,sizeof(buff));

                    write(fd2,buff, sizeof(buff));

                    totalSize -= blockSize;
                }
            }

            struct utimbuf times;

            times.actime = tempMeta.mtime;
            times.modtime = tempMeta.mtime;
            

            utime(filename,&times);


            close(fd2);

            close(fd);
            return;
        }
        lseek(fd,tempMeta.size,SEEK_CUR);

        if( (tempMeta.size % 2) != 0 ){
            char lineBreak[1];
            read(fd,lineBreak,1);
        }

    }

    close(fd);


}

void option_t(char* archFile){
    int fd = open(archFile, O_RDWR);

    if(fd == -1){
        printf("Error Archive File Not Found \n");
        return;
    }

    struct stat buffSize;

    stat(archFile,&buffSize);
    
    struct ar_hdr arHdr;
    struct meta tempMeta;

    char test[8];
    read(fd,&test,8);

    if(strcmp(test,"!<arch>\n") != 0){
        printf("Wrong File Format \n");
        return;
    }

    while( (read(fd,&arHdr,60)) != 0 ){
        fill_meta(arHdr,&tempMeta);

        lseek(fd,tempMeta.size,SEEK_CUR);

        for (int i = 0; i< strlen(tempMeta.name); i++){
            if(tempMeta.name[i] == '/'){
                tempMeta.name[i] =  ' ';
            }
        }

        tempMeta.name[16] = ' ';
        tempMeta.name[17] = ' ';
        tempMeta.name[18] = ' ';
        printf("%s \n",tempMeta.name);

        if( (tempMeta.size % 2) != 0 ){
            char lineBreak[1];
            read(fd,lineBreak,1);
        }
    }

    close(fd);
}


int main(int argc, char *argv[]){

    

    char* key  = argv[1];
    char* archFile  = argv[2];
    char* filename= argv[3];

    if (strcmp(key,"q") == 0){
        if( argc != 4 ){
            printf("Sorry wrong number of arguements \n");
            return 0;
        }
        option_q(archFile,filename);
        printf("Choice: %s \n", key);
    }
    else if (strcmp(key,"xo") == 0){
        if( argc != 4 ){
            printf("Sorry wrong number of arguements \n");
            return 0;
        }
        option_xo(archFile,filename);
        printf("Choice: %s \n", key);
    }
    else if (strcmp(key,"t") == 0){
        if( argc != 3 ){
            printf("Sorry wrong number of arguements \n");
            return 0;
        }
        option_t(archFile);
        printf("Choice: %s \n", key);
    }
    else if (strcmp(key,"A") == 0){
        if( argc != 3 ){
            printf("Sorry wrong number of arguements \n");
            return 0;
        }
        option_A(archFile);
        printf("Choice: %s \n", key);
    }    
    else{
        printf("Invalid key command \n");
    }

    return 0;
}

