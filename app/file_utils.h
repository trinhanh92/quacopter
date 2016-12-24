#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include "common.h"

int
file_write(u8_t* data_buff, u32_t buff_size);

int
file_read(u8_t* data_buff, u32_t buff_size);

#endif
