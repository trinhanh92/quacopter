#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "jsmn.h"

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


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
/******************************************************************************
* @brief function to get value from json format with specific key
*
* @param[in]  json_string - json string input
* @param[in]  len         - length of json string
* @param[in]  key         - key to find value
* @param[out] value_ret   - value return corresponding key
*/
int json_parser(const char *json_string, int len, const char *key, char *value_ret)
{
    int i;
    int r;
    jsmn_parser parser;
    jsmntok_t   t[128];

    jsmn_init(&parser);
    r = jsmn_parse(&parser, json_string, len, t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        return -1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        return -1;
    }


    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(json_string, &t[i], key) == 0) {
            /* We may use strndup() to fetch string value */
            // printf("[json] - value return: %.*s\n", t[i+1].end-t[i+1].start,
            //         json_string + t[i+1].start);
            memcpy(value_ret, json_string + t[i+1].start,  t[i+1].end-t[i+1].start);
            i++;
            break;
        }
    }
    if ('\0' == value_ret[0]) {
        return -1;
    }
    return 0;
}
/******************************************************************************
* @brief Get value request string with specific key
*
* @param[in]  request     - post data request strin 
* @param[in]  req_len     - length of request string
* @param[in]  key         - key to find value
* @param[out] value       - value return corresponding key
* @return     0 if success, other for fail
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

    } else if (0 == strcmp(key, "direct")) {
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
