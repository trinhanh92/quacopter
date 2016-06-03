#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

/******************************************************************************
* @brief This function used to encrypt a plain text string to MD5 
*   
* @param[in] str - string buffer
* @param[in] lenght - string buffer length
* @return    MD5 string
*/
char *str2md5(const char *str, int length)
{
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}


/******************************************************************************
* @brief Parse client request and get parameters 
*   
* @param[in] str - string buffer
* @param[in] lenght - string buffer length
* @return    MD5 string
*/
int
parse_request(char *request, int req_len, char *key, char *value)
{
    char *and_ptr = NULL;
    char *key_ptr = NULL;
    int  value_len = 0;
    char *temp_ptr = request;

    // string is NULL
    if(NULL == request)
    {
        return -1;
    }

    if (0 == strcmp(key, "sig")) {
        key_ptr = strstr(temp_ptr, key);    // search 'sig' position
        if (NULL == key_ptr) {
            return -1;
        }
        value_len = (req_len - (key_ptr - temp_ptr) - strlen(key) - 1); // minus len of key and an '=' char

    } else if ((0 == strcmp(key, "x")) || 
               (0 == strcmp(key, "y")) || 
               (0 == strcmp(key, "z"))) {
        key_ptr = strstr(temp_ptr, key);    // search key position
    
        if (NULL == key_ptr) {
            return -1;                      // have no key in string
        }
        and_ptr =  strstr(key_ptr, "&");    // search '&' position
        value_len = and_ptr - key_ptr - strlen(key) - 1;                // minus len of key and an '='

    } else {
        return -1;
    }

    strncpy(value, key_ptr + strlen(key) + 1, value_len);   // + len of key and +1 for '='
    printf("value of %s is: %s\n", key, value);
    return 0;

}
