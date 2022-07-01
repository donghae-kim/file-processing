#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char*argv[]){

    if(argc != 4){
        fprintf(stderr, "file number wrong\n");
		exit(1);
    }

    int offset = atoi(argv[1]);
    int bytes = atoi(argv[2]);
    char *filename = argv[3];
    int fd1;

    if((fd1 = open(filename,O_RDWR))<0){
        fprintf(stderr, "open error for %s\n",filename);
        exit(1);
    }
    int length_file = lseek(fd1,0,SEEK_END);
    char *buffer[1024];


    
    if(bytes > 0 ){       
        if(offset+bytes>length_file){
            bytes= length_file-offset;
        } 
        lseek(fd1, offset+bytes, SEEK_SET);
        read(fd1,buffer,length_file-offset-bytes);
        lseek(fd1, offset, SEEK_SET);
        write(fd1,buffer,length_file-offset-bytes);
        ftruncate(fd1,length_file-bytes);
    }
    else if(bytes < 0 ){        
        if(offset+bytes<0){
            bytes=-offset;
        }
        lseek(fd1, offset, SEEK_SET);
        read(fd1,buffer,length_file-offset);

        lseek(fd1, offset+bytes, SEEK_SET);
        write(fd1,buffer,length_file-offset-bytes);
        ftruncate(fd1,length_file+bytes);
    }


}