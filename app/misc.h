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

/******************************************************************************
* @brief function to get value from json format with specific key
*
* @param[in]  json_string - json string input
* @param[in]  len         - length of json string
* @param[in]  key         - key to find value
* @param[out] value_ret  - value return corresponding key
*/
int json_parser(const char *json_string, int len, const char *key, char *value_return);
#endif
