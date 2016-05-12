#ifndef __MISC_H__
#define __MISC_H__

#define SERCRET_KEY "drone_hung_hue@uit.edu.vn"

/******************************************************************************
* @brief This function used to encrypt a plain text o MD5 
* 	
* @param[in] str - string buffer
* @param[in] length - string buffer length
* @return    MD5 string
*/
char *str2md5(const char *str, int length);

#endif
