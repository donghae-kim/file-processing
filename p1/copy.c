#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char* argv[]){

    
    char* Fileread, *Filewrite;
    Fileread = argv[1];
	Filewrite = argv[2];

    int fdread, fdwrite,length;
    char buffer[10] = {0,};

    if (argc != 3) {
		fprintf(stderr, "file number wrong\n");
		exit(1);
	}

    if ((fdread = open(Fileread, O_RDONLY)) < 0) {
		fprintf(stderr, "open failed: %s\n", Fileread);
		exit(1);
	}

    if ((fdwrite = open(Filewrite, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
		fprintf(stderr, "write failed: %s\n", Filewrite);
		exit(1);
	}

   while ((length = read(fdread, &buffer, 10)) > 0){
		write(fdwrite, &buffer, length);
	}

}