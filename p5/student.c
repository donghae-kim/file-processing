#include <stdio.h>		// 필요한 header file 추가 가능
#include <string.h>
#include <stdlib.h>
#include "student.h"
void pack(char *recordbuf, const STUDENT *s);
void unpack(const char *recordbuf, STUDENT *s);
void printRecord(const STUDENT *s);
enum FIELD getFieldID(char *fieldname);
//
// 함수 readRecord()는 학생 레코드 파일에서 주어진 rrn에 해당하는 레코드를 읽어서 
// recordbuf에 저장하고, 이후 unpack() 함수를 호출하여 학생 타입의 변수에 레코드의
// 각 필드값을 저장한다. 성공하면 1을 그렇지 않으면 0을 리턴한다.
// unpack() 함수는 recordbuf에 저장되어 있는 record에서 각 field를 추출하는 일을 한다.
//
int readRecord(FILE *fp, STUDENT *s, int rrn){
	char recordbuf[RECORD_SIZE];
	if(fread(recordbuf,RECORD_SIZE,1,fp)==EOF){
		return 0;	
	}
	unpack(recordbuf,s);
	return 1;
}
void unpack(const char *recordbuf, STUDENT *s){
	int idx=0;
	char tmp[RECORD_SIZE]=" ";
	strcpy(tmp,recordbuf);
	char *arr[5];
	char *arr_tmp[5];
	memset(arr_tmp,0,1);
	char *token;
	token = strtok(tmp,"#");   // There are two delimiters here*/
    
	while (token != NULL){
        arr[idx++] = token;
        token = strtok(NULL, "#");
    }
	

	sprintf(s->id,"%s",arr[0]);
	sprintf(s->name,"%s",arr[1]);
	sprintf(s->dept,"%s",arr[2]);
	sprintf(s->addr,"%s",arr[3]);
	sprintf(s->email,"%s",arr[4]);
}

//
// 함수 writeRecord()는 학생 레코드 파일에 주어진 rrn에 해당하는 위치에 recordbuf에 
// 저장되어 있는 레코드를 저장한다. 이전에 pack() 함수를 호출하여 recordbuf에 데이터를 채워 넣는다.
// 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int writeRecord(FILE *fp, const STUDENT *s, int rrn){
	fseek(fp,HEADER_SIZE,SEEK_SET);
	
	fseek(fp,RECORD_SIZE*rrn,SEEK_CUR);
	char recordbuf[RECORD_SIZE];
	pack(recordbuf,s);

	if(fwrite(recordbuf,RECORD_SIZE,1,fp)==EOF){
		return 0;
	}
	else
		return 1;
}
void pack(char *recordbuf, const STUDENT *s){
		sprintf(recordbuf,"%s#%s#%s#%s#%s#",s->id,s->name,s->dept,s->addr,s->email);
}

//
// 함수 appendRecord()는 학생 레코드 파일에 새로운 레코드를 append한다.
// 레코드 파일에 레코드가 하나도 존재하지 않는 경우 (첫 번째 append)는 header 레코드를
// 파일에 생성하고 첫 번째 레코드를 저장한다. 
// 당연히 레코드를 append를 할 때마다 header 레코드에 대한 수정이 뒤따라야 한다.
// 함수 appendRecord()는 내부적으로 writeRecord() 함수를 호출하여 레코드 저장을 해결한다.
// 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email){

	fseek(fp,0L,SEEK_SET);
	int rrn=0;
	int tmp=0;

	if((fscanf(fp,"%d",&rrn))==EOF){
		fprintf(fp,"%-4d-1   ",rrn);
	}
	else{
		fseek(fp,0L,SEEK_SET);
		fscanf(fp,"%d",&rrn);
		fscanf(fp,"%d",&tmp);
		rrn++;
		fseek(fp,0L,SEEK_SET);
		fprintf(fp,"%-4d%-4d",rrn,tmp);
	}
	struct _STUDENT s;
	strcpy(s.id,id);
	strcpy(s.name,name);
	strcpy(s.dept,dept);
	strcpy(s.addr,addr);
	strcpy(s.email,email);
	if(writeRecord(fp,&s,rrn)==1){
		return 1;
	}
	else
		return 0;
}

//
// 학생 레코드 파일에서 검색 키값을 만족하는 레코드가 존재하는지를 sequential search 기법을 
// 통해 찾아내고, 이를 만족하는 모든 레코드의 내용을 출력한다. 검색 키는 학생 레코드를 구성하는
// 어떤 필드도 가능하다. 내부적으로 readRecord() 함수를 호출하여 sequential search를 수행한다.
// 검색 결과를 출력할 때 반드시 printRecord() 함수를 사용한다. (반드시 지켜야 하며, 그렇지
// 않는 경우 채점 프로그램에서 자동적으로 틀린 것으로 인식함)
//
void searchRecord(FILE *fp, enum FIELD f, char *keyval){
	int maxrrn=0;
	fseek(fp,0L,SEEK_SET);
	fscanf(fp,"%d",&maxrrn);

	struct _STUDENT s;
	fseek(fp,HEADER_SIZE,SEEK_SET);
	for(int i=0; i<=maxrrn; i++){
		readRecord(fp,&s,i);
		switch(f){
			case 0:
				if(strcmp(s.id,keyval)==0){
					printRecord(&s);
				}
			case 1:
				if(strcmp(s.name,keyval)==0){
					printRecord(&s);
				}
			case 2:
				if(strcmp(s.dept,keyval)==0){
					printRecord(&s);
				}
			case 3:
				if(strcmp(s.addr,keyval)==0){
					printRecord(&s);
				}
			case 4:
				if(strcmp(s.email,keyval)==0){
					printRecord(&s);
				}
		}
		
	}
}
void printRecord(const STUDENT *s){
	printf("%s | %s | %s | %s | %s\n", s->id, s->name, s->dept, s->addr, s->email);
}

//
// 레코드의 필드명을 enum FIELD 타입의 값으로 변환시켜 준다.
// 예를 들면, 사용자가 수행한 명령어의 인자로 "NAME"이라는 필드명을 사용하였다면 
// 프로그램 내부에서 이를 NAME(=1)으로 변환할 필요성이 있으며, 이때 이 함수를 이용한다.
//
enum FIELD getFieldID(char *fieldname){
	enum FIELD f;
	if(strcmp(fieldname,"ID")==0){
		f= ID;
	}
	else if(strcmp(fieldname,"NAME")==0){
		f= NAME;
	}
	else if(strcmp(fieldname,"DEPT")==0){
		f= DEPT;
	}
	else if(strcmp(fieldname,"ADDR")==0){
		f= ADDR;
	}
	else if(strcmp(fieldname,"EMAIL")==0){
		f= EMAIL;
	}
	return f;
}


//
// 학생 레코드 파일에서 "학번" 키값을 만족하는 레코드를 찾아서 이것을 삭제한다.
// 참고로, 검색 조건은 반드시 학번(ID)만을 사용한다. 레코드의 검색은 searchRecord() 함수와
// 유사한 방식으로 구현한다. 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//

int deleteRecord(FILE *fp, enum FIELD f, char *keyval){
	int maxrrn=0;
	int delrecord;

	fseek(fp,0L,SEEK_SET);
	fscanf(fp,"%4d",&maxrrn);

	struct _STUDENT s;
	fseek(fp,HEADER_SIZE,SEEK_SET);
	for(int i=0; i<=maxrrn; i++){
		int state = ftell(fp);
		int state_rrn = (state -8)/85;
		readRecord(fp,&s,i);
		switch(f){
			case 0:
				if(strcmp(s.id,keyval)==0){
					int tmp;
					fseek(fp,4,SEEK_SET);
					fscanf(fp,"%4d",&tmp);

					fseek(fp,4,SEEK_SET);
					char buf[5]= " ";
					sprintf(buf,"%-4d",state_rrn);
					fwrite(buf,4,1,fp);

					fseek(fp,state,SEEK_SET);
					char bufs[6]= " ";
					sprintf(bufs,"*%-4d",tmp);
					fwrite(bufs,5,1,fp);

					fseek(fp,state,SEEK_SET);
					return 1;
				}
		}
		if(i==maxrrn){
			return 0;
		}
	}
}

//
// 학생 레코드 파일에 새로운 레코드를 추가한다. 과제 설명서에서 언급한 대로, 삭제 레코드가
// 존재하면 반드시 삭제 레코드들 중 하나에 새로운 레코드를 저장한다. 삭제 레코드 리스트 관리와
// 삭제 레코드의 선택은 교재 방식을 따른다. 새로운 레코드의 추가는 appendRecord() 함수와 비슷한
// 방식으로 구현한다. 성공적으로 수행하면 '1'을, 그렇지 않으면 '0'을 리턴한다.
//
int insertRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email){
	fseek(fp,0L,SEEK_SET);
	int rrn=0;
	int tmp=0;

	fscanf(fp,"%d",&rrn);
	fseek(fp,0L,SEEK_SET);
	fscanf(fp,"%d",&rrn);
	fscanf(fp, "%d",&tmp);
	
	struct _STUDENT s;
	strcpy(s.id,id);
	strcpy(s.name,name);
	strcpy(s.dept,dept);
	strcpy(s.addr,addr);
	strcpy(s.email,email);
	fseek(fp,8+tmp*85,SEEK_SET);
	char buf[5]=" ";
	fscanf(fp,"%s",buf);
	for(int i=0; i<4; i++){
		buf[i]=buf[i+1];
	}

	fseek(fp,4,SEEK_SET);
	char bufs[5]= " ";
	sprintf(bufs,"%-4s",buf);
	fwrite(bufs,4,1,fp);
 
	
	if(writeRecord(fp,&s,tmp)==1){
		return 1;
	}
	else
		return 0;

}


void main(int argc, char *argv[])
{
	FILE *fp;			// 모든 file processing operation은 C library를 사용할 것
	if((fp = fopen(argv[2],"a+"))==NULL){
		fprintf(stderr,"open error");
		exit(1);
	}
	fclose(fp);

	if((fp = fopen(argv[2],"r+"))==NULL){
		fprintf(stderr,"open error");
		exit(1);
	}


	if(strcmp(argv[1],"-a")==0){
		if(argc==8 && strlen(argv[3])<=8 && strlen(argv[4])<=12 && strlen(argv[5])<=10 && strlen(argv[6])<=30&&strlen(argv[7])<=20 ){
		appendRecord(fp,argv[3],argv[4],argv[5],argv[6],argv[7]);
		}
		else{
		fprintf(stderr,"argc error");
		exit(1);
		}
	}
	else if(strcmp(argv[1],"-s")==0){
		char field_name[100]="";
		char field_value[100]="";
		int flag=0;
		int j=0;
		for(int i=0; i<strlen(argv[3]); i++){
			if(argv[3][i]=='='){
				flag=1;
				j=0;
				continue;
			}
			if(flag==0){
				field_name[j++]=argv[3][i];	
			}
			else if(flag ==1){
				field_value[j++]=argv[3][i];
			}
		}
		enum FIELD f;
		f=getFieldID(field_name);
		searchRecord(fp,f,field_value);
	}
	else if(strcmp(argv[1],"-d")==0){
		char field_name[100]="";
		char field_value[100]="";
		int flag=0;
		int j=0;
		for(int i=0; i<strlen(argv[3]); i++){
			if(argv[3][i]=='='){
				flag=1;
				j=0;
				continue;
			}
			if(flag==0){
				field_name[j++]=argv[3][i];	
			}
			else if(flag ==1){
				field_value[j++]=argv[3][i];
			}
		}
		enum FIELD f;
		f=getFieldID(field_name);
		deleteRecord(fp,f,field_value);
	}
	else if(strcmp(argv[1],"-i")==0){
		int delrrn=0;
		fseek(fp,4,SEEK_SET);

		if(argc==8 && strlen(argv[3])<=8 && strlen(argv[4])<=12 && strlen(argv[5])<=10 && strlen(argv[6])<=30&&strlen(argv[7])<=20 ){
		if(fscanf(fp,"%d",&delrrn)==EOF || delrrn == -1){
			appendRecord(fp,argv[3],argv[4],argv[5],argv[6],argv[7]);
		}
		else{
			insertRecord(fp,argv[3],argv[4],argv[5],argv[6],argv[7]);
		}
		}
		else{
		fprintf(stderr,"argc error");
		exit(1);
		}
	}
	
	fclose(fp);
}
