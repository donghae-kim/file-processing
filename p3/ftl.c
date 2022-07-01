#define PRINT_FOR_DEBUG

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include "blkmap.h"

AddrMapTbl addrmaptbl;
SpareData sdata;
extern FILE *devicefp;
bool check_pbn[BLOCKS_PER_DEVICE];
int reserved_empty_blk = DATABLKS_PER_DEVICE;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void print_block(int pbn);
void print_addrmaptbl();
int dd_read(int, char *);
int dd_write(int, char *);
int dd_erase(int);
//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다
//
void ftl_open()
{
	
	int i,lbn;
	char pagebuf[PAGE_SIZE];
	// initialize the address mapping table

	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		addrmaptbl.pbn[i] = -1;
	}

	for(int i=0;i<BLOCKS_PER_DEVICE;i++){
			dd_read(i* PAGES_PER_BLOCK ,pagebuf);
			memcpy(&lbn,pagebuf+SECTOR_SIZE,4);
			if(lbn >= 0){
				addrmaptbl.pbn[lbn] = i;
				check_pbn[i] = true;
			}
			else{
				addrmaptbl.pbn[DATABLKS_PER_DEVICE] = i;
			}
	}
	return;
}

// file system을 위한 FTL이 제공하는 write interface
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'이며, 호출하는 쪽에서 미리 메모리를 할당받아야 함
//
void ftl_write(int lsn, char *sectorbuf)
{
/*
#ifdef PRINT_FOR_DEBUG			// 필요 시 현재의 block mapping table을 출력해 볼 수 있음
	print_addrmaptbl();
#endif*/

	//
	// block mapping 기법에서 overwrite가 발생하면 이를 해결하기 위해 반드시 하나의 empty block이
	// 필요하며, 초기값은 flash memory에서 맨 마지막 block number를 사용함
	// overwrite를 해결하고 난 후 당연히 reserved_empty_blk는 overwrite를 유발시킨 (invalid) block이 되어야 함
	// 따라서 reserved_empty_blk는 고정되어 있는 것이 아니라 상황에 따라 계속 바뀔 수 있음
	//

	sdata.lsn = lsn;

	char pagebuf[PAGE_SIZE];
	int off,lbn,pbn,ppn,i,psn;
	reserved_empty_blk = DATABLKS_PER_DEVICE;
	
	off = lsn % PAGES_PER_BLOCK;
	lbn = lsn / PAGES_PER_BLOCK;
	pbn = addrmaptbl.pbn[lbn];
	ppn = pbn * PAGES_PER_BLOCK + off;

	
	if(addrmaptbl.pbn[lbn]>=0){
		int psn = pbn * PAGES_PER_BLOCK + off;				
		int flag;
		int ppne;
		int pbne;

		dd_read(ppn,pagebuf);
		memcpy(&flag,&pagebuf[SECTOR_SIZE],sizeof(int));

		if(flag >= 0){			//overwrite시작
		pbne = addrmaptbl.pbn[DATABLKS_PER_DEVICE];
		
			for(int j=0;j<PAGES_PER_BLOCK;j++){
				psn = pbn * PAGES_PER_BLOCK + j;
				ppne = pbne * PAGES_PER_BLOCK + j;
				dd_read(psn,pagebuf);
				if(j == off){
					memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
					memcpy(&pagebuf[SECTOR_SIZE],&sdata,sizeof(int));
					dd_write(ppne,pagebuf);
				}
				else{
					dd_write(ppne,pagebuf);
				}
			}
			
			addrmaptbl.pbn[lbn] = addrmaptbl.pbn[DATABLKS_PER_DEVICE];
			addrmaptbl.pbn[DATABLKS_PER_DEVICE] = pbn;
			reserved_empty_blk = pbn;
			dd_erase(pbn);
		}
		else{
			memcpy(pagebuf,sectorbuf,SECTOR_SIZE);
			memcpy(&pagebuf[SECTOR_SIZE],&sdata,sizeof(int));
			dd_write(ppn,pagebuf);
		}
	}
	else{		//처음 들어왔을때 것							
		for(int i=0;i<BLOCKS_PER_DEVICE;i++){
			if(check_pbn[i]==false){
				check_pbn[i] = true;
				addrmaptbl.pbn[lbn] = i;
				break;
			}
		}
		
		pbn = addrmaptbl.pbn[lbn];
		ppn = pbn * PAGES_PER_BLOCK + off;


		memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
		memcpy(&pagebuf[SECTOR_SIZE], &sdata, 4);
		dd_write(ppn,pagebuf);
	}

	return;
}

//
// file system을 위한 FTL이 제공하는 read interface
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'이며, 호출하는 쪽에서 미리 메모리를 할당받아야 함
// 
void ftl_read(int lsn, char *sectorbuf)
{
	
	int lbn,pbn,ppn,off;
	char buf[PAGE_SIZE];
	lbn = lsn / PAGES_PER_BLOCK;
	off = lsn % PAGES_PER_BLOCK;
	pbn = addrmaptbl.pbn[lbn];
	ppn = pbn * PAGES_PER_BLOCK + off;
	if(dd_read(ppn,buf)==1){
		memcpy(sectorbuf,buf,SECTOR_SIZE);
	}
	return; 	
}

//
// for debugging
//
void print_block(int pbn)
{
	SpareData *sdata;
	char *pagebuf;

	int i;
	sdata = (SpareData *)malloc(SPARE_SIZE);

	pagebuf = (char *)malloc(PAGE_SIZE);

	printf("Physical Block Number: %d\n", pbn);
	for(i = pbn*PAGES_PER_BLOCK; i < (pbn+1)*PAGES_PER_BLOCK; i++)
	{
		dd_read(i, pagebuf);
		memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
		printf("\t   %5d-[%7d]\n", i, sdata->lsn);
	}

	free(pagebuf);
	free(sdata);

	return;
}

//
// for debugging
//

void print_addrmaptbl()
{
	int i;

	printf("Address Mapping Table: \n");
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		if(addrmaptbl.pbn[i] >= 0)
		{
			printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
		}
	}
}
	