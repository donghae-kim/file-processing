#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if(argc != 4){
        fprintf(stderr, "file number wrong\n");
		exit(1);
    }

    int offset = atoi(argv[1]);
    char *data = argv[2];
    char *filename = argv[3];
    int fd1;
    char *buffer[1024];

    
    if((fd1 = open(filename,O_RDWR))<0){
        fprintf(stderr, "open error for %s\n",filename);
        exit(1);
    }

     int length_data = strlen(data);
    int length_file = lseek(fd1,0,SEEK_END);

    if (lseek(fd1, offset, SEEK_SET) < 0){	//offset으로 파일 위치 이동
		fprintf(stderr, "lseek error\n");
		exit(1);
	}
    
    while((read(fd1,buffer,sizeof(buffer))) >0){
    }

    if ((lseek(fd1, offset, SEEK_SET)) < 0){	//offset으로 파일 위치 이동
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

    write(fd1,data,length_data);
    write(fd1,buffer,length_file-offset);

    close(fd1);

    exit(0);

}