#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;

int main( )
{
	// 아래 변수는 테스트할 때 필요하면 사용하기 바람
	char sectorbuf[SECTOR_SIZE], *blockbuf;
	int i, lsn;

	// flash memory의 파일명은 'flashmemory'을 가정함
	devicefp = fopen("flashmemory", "w+b");	

	if(devicefp == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	//
	// initialize flash memory where each byte are set to '0xFF'
	// 
	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	ftl_open();
printf("0번 넣음\n");
	memset((void*)sectorbuf,'0',SECTOR_SIZE);
	ftl_write(0,sectorbuf);
	print_addrmaptbl();
print_block(0);
print_block(15);

printf("\n1번 넣음\n");
	memset((void*)sectorbuf,'1',SECTOR_SIZE);
	ftl_write(1,sectorbuf);
	print_addrmaptbl();
print_block(0);
print_block(15);

printf("\n1번 다시 넣음\n");
	memset((void*)sectorbuf,'1',SECTOR_SIZE);
	ftl_write(1,sectorbuf);
	print_addrmaptbl();
print_block(0);
print_block(15);

printf("\n1번 또 다시 넣음\n");
	memset((void*)sectorbuf,'*',SECTOR_SIZE);
	ftl_write(1,sectorbuf);
	print_addrmaptbl();
print_block(0);
print_block(15);


	free(blockbuf);
	fclose(devicefp);

	return 0;
}