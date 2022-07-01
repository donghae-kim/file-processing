#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int offset = atoi(argv[1]);	//오프셋 int형으로
	int bytes = atoi(argv[2]);	//바이트 읽어와서 int형으로
	char* filename = argv[3];	//3번째 인자 filename으로
	int fd;
	char c;

	if (argc != 4){	//오프셋 바이트 파일이름 다받았는지
		fprintf(stderr, "argc number error!\n");
		exit(1);
	}

	if ((fd = open(filename, O_RDONLY)) < 0){	//filename 읽기전용 오픈
			fprintf(stderr, "error for open %s\n", argv[1]);
			exit(1);
	}
	int length_file = lseek(fd,0,SEEK_END);


	if (lseek(fd, offset, SEEK_SET) < 0){	//offset으로 파일 위치 이동
		fprintf(stderr, "lseek error\n");
		exit(1);
	}


	if(bytes>0){
		if(offset + bytes>length_file){
			bytes= length_file-offset;
		}
		while (bytes--){
			if (read(fd, &c, 1)==0)
				break;
			printf("%c", c);
			}
	}
	else if(bytes<0){
		if(offset+bytes<0){
            bytes=-offset;
        }
		if (lseek(fd, offset+bytes, SEEK_SET) < 0){	//offset으로 파일 위치 이동
		fprintf(stderr, "lseek error\n");
		exit(1);
		}
	
		int nbytes = -bytes;	//음수인 bytes 절대값 변환

		while (nbytes--){
			if (read(fd, &c, 1)==0)
				break;
			printf("%c", c);
			}
	}

	close(fd);
	exit(0);
}