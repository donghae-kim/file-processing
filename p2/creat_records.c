#include<stdio.h>
#include<stdlib.h>
// 필요하면 header 파일 추가 가능

//
// input parameters: 학생 레코드 수, 레코드 파일
//
int main(int argc, char **argv)
{
	
	// 사용자로부터 입력 받은 레코드 수 만큼의 레코드 파일을 생성하는 코드 구현
	FILE *fp;

	int record_num = atoi(argv[1]);
	char record[200];

	for(int i=0; i<200; i++){
		record[i] = 'a';
	}

	if((fp = fopen(argv[2],"wb"))==NULL){
		fprintf(stderr,"file open error");
		exit(1);
	}

	for(int i=0; i<record_num; i++){
		fwrite(record,200,1,fp);
	}

	fclose(fp);

	return 0;
}
