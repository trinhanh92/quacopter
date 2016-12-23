#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int
file_write(u8_t* data_buff, u32_t buff_size) 
{
	FILE *fp;
	int i;
	fp = fopen("config.txt", "wb");
	if (fp == NULL) {
	 printf("I couldn't open config.txt for writing.\n");
	 return -1;
	}
	if (NULL == data_buff || buff_size == 0) {
		return -1;
	}	
	/** write to file */
	for (i = 0; i < buff_size; i++) {
		fwrite(data_buff, 1, 1, fp);
	}
	/** close file */
	fclose(fp);
	return 0;
}

int
file_read(u8_t* data_buff, u32_t buff_size)
{
	FILE *fp;
	int i;
	fp = fopen("config.txt", "rb+");
	if (fp == NULL) {
	 printf("I couldn't open config.txt for reading.\n");
	 return -1;
	}
	if (NULL == data_buff || buff_size == 0) {
		return -1;
	}	
	/** read from file */
	for (i = 0; i < buff_size; i++) {
		fread(data_buff, 1, buff_size, fp);
	}
	/** close file */
	fclose(fp);
	return 0;
}
