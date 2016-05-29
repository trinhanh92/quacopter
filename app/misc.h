#ifndef __MISC_H__
#define __MISC_H__

#define SERCRET_KEY "drone_hung_hue@uit.edu.vn"

/* data from client request */
typdef struct req_data_s {
	int x;				// value of x
	int y;				// value of y
	int z;				// value of z
	char sig[33];		// signature in MD5
} req_data_t;

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
