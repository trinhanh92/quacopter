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
parse_request(char *request, int req_len, req_data_t *data)
{
    char *and_ptr;
    char *key_ptr;
    char temp[33] = {0};
    char *temp_ptr = request;
    // search 'x' 
    key_ptr = strstr(temp_ptr, "x=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NUL == and_ptr) {
        return -1;
    }
    // get x value
    if ((and_ptr - key_ptr) > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, sizeof temp, key_ptr);
        data->x = atoi(temp);
    } else {
        return -1;
    }

    temp_ptr += and_ptr; 
    // search 'y' 
    key_ptr = strstr(temp_ptr, "y=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NUL == and_ptr) {
        return -1;
    }
    // get y value
    if ((and_ptr - key_ptr) > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, sizeof temp, key_ptr);
        data->y = atoi(temp);
    } else {
        return -1;
    }

    temp_ptr += and_ptr; 
    // search 'z' 
    key_ptr = strstr(temp_ptr, "z=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NUL == and_ptr) {
        return -1;
    }
    // get z value
    if ((and_ptr - key_ptr) > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, sizeof temp, key_ptr);
        data->z = atoi(temp);
    } else {
        return -1;
    }

    temp_ptr += and_ptr; 
    // search 'sig' 
    key_ptr = strstr(temp_ptr, "x=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // get sig value
    if ((req_len - (and_ptr - request)) > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, sizeof temp, key_ptr);
        strncpy(data->sig, 32, temp);
    } else {
        return -1;
    }
    return 0;
}
