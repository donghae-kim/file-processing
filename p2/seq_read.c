#include <stdio.h>
#include <sys/time.h>
#include<stdlib.h>
#include <string.h>
//필요하면 header file 추가 가능


//
// input parameters: 레코드 파일
//
int main(int argc, char **argv)
{

	// 레코드 파일로부터 전체 레코드를 순차적으로 읽어들이고, 이때
	// 걸리는 시간을 측정하는 코드 구현

    char buf[200];
    FILE *fp;
    struct timeval startTime, endTime;

    if((fp=fopen(argv[1],"rb"))==NULL){
        fprintf(stderr,"file open error\n");
        exit(1);
    }
    gettimeofday(&startTime, NULL);
    int i=1;
    while(1){

        fread(buf, sizeof(buf), 1, fp); 
        if(feof(fp)!=0){
            break;
        }
    }
    gettimeofday(&endTime, NULL);

    printf("%ld usec\n", (endTime.tv_sec - startTime.tv_sec)*
			1000000 + endTime.tv_usec - startTime.tv_usec);
    fclose(fp);
	return 0;
}
