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
    int  value_len;
    char *temp_ptr = request;
    // search 'x' 
    key_ptr = strstr(temp_ptr, "x=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NULL == and_ptr) {
        return -1;
    }
    // get x value
    value_len = (and_ptr - key_ptr);
    if (value_len > 0) {
        printf("value len: %d\n", value_len);
        memset(temp, 0, sizeof temp);
        strncpy(temp, key_ptr + 1, sizeof value_len);
        data->x = atoi(temp);
    } else {
        return -1;
    }

    printf("gotta X: %d\n", data->x);
    temp_ptr = and_ptr + 1;

    // search 'y' 
    key_ptr = strstr(temp_ptr, "y=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NULL == and_ptr) {
        return -1;
    }
    // get y value
    value_len = (and_ptr - key_ptr - 1);
    if (value_len > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, key_ptr + 1, sizeof value_len);
        data->y = atoi(temp);
    } else {
        return -1;
    }

    printf("gotta Y: %d\n", data->y);
    temp_ptr = and_ptr + 1; 
    // search 'z' 
    key_ptr = strstr(temp_ptr, "z=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // search '&'
    and_ptr = strstr(temp_ptr, "&");
    if(NULL == and_ptr) {
        return -1;
    }
    // get z value
    value_len = (and_ptr - key_ptr - 1);
    if (value_len > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, key_ptr + 1, sizeof value_len);
        data->z = atoi(temp);
    } else {
        return -1;
    }

    printf("gotta Z: %d\n", data->z);
    temp_ptr = and_ptr; 
    // search 'sig' 
    key_ptr = strstr(temp_ptr, "x=");
    if (NULL == key_ptr) {
       return -1; 
    }
    // get sig value
    value_len = (req_len - (key_ptr - request + 1));
    if (value_len > 0) {
        memset(temp, 0, sizeof temp);
        strncpy(temp, key_ptr + 1, sizeof value_len);
        strncpy(data->sig, temp, sizeof temp);
    } else {
        return -1;
    }
    printf("gotta sig: %s\n", data->sig);
    return 0;
}
