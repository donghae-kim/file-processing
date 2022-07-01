#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf(stderr, "file number wrong\n");
		exit(1);
    }

    char *file_1 = argv[1];
    char *file_2 = argv[2];
    char *file_3 = argv[3];
    int fd1, fd2,fd3;
    char c;

    if((fd1 = open(file_1, O_RDONLY )) < 0){
        fprintf(stderr, "error for open %s\n", file_1);
		exit(1);
	}
    if((fd2 = open(file_2, O_RDONLY )) < 0){
        fprintf(stderr, "error for open %s\n", file_2);
		exit(1);
	}
    if((fd3 = open(file_3, O_APPEND |  O_RDWR | O_CREAT | O_TRUNC , 0666)) < 0){
        fprintf(stderr, "error for open %s\n", file_3);
		exit(1);
	}

    while (1){
		if (read(fd1, &c, 1) == 0){
             while (1){
                if (read(fd2, &c, 1) == 0)
                    break;
                write(fd3, &c, 1);
            }
			break;
        }
		write(fd3, &c, 1);
	}

     close(fd1);
	close(fd2);
    close(fd3);
	
	exit(0);
}