#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

/******************************************************************************
* @brief Convert 2 byte data to single byte array
*   
* @param[in]  inp_num    - input number
* @param[out] byte_arr   - output array
* @return     None
*/
void
short2byte(i16_t inp_num, u8_t *byte_arr)
{
    byte_arr[0] = ((inp_num >> 8) & 0xFF);
    byte_arr[1] = ((inp_num) & 0xFF);
    // byte_arr[0] = 0;
    // byte_arr[1] = 100;
    printf("%04X | %02X %02X\n", inp_num, byte_arr[0], byte_arr[1]);
}


/******************************************************************************
* @brief Package data to send to SPI slave
*   
* @param[in]  req_data      - request data
* @param[out] send_data     - send data buffer in hex
* @param[in]  send_data_len - length of send data in bytes
* @return     None
*/
void
spi_data_to_send(req_data_t req_data, u8_t *send_data, int send_data_len)
{
    int i;
    u8_t x_buf[2];
    u8_t y_buf[2];
    u8_t z_buf[2];

    // convert number to single byte array
    short2byte(req_data.x, x_buf);
    short2byte(req_data.y, y_buf);
    short2byte(req_data.z, z_buf);

    // join data into a output array to send througn SPI
    memset(send_data, 0, send_data_len);
    memcpy(send_data, x_buf, sizeof x_buf);
    memcpy(send_data + sizeof x_buf, y_buf, sizeof y_buf);
    memcpy(send_data + sizeof x_buf + sizeof y_buf, z_buf, sizeof z_buf);

    // show result
    printf("\nspi send data: ");
    for (i = 0; i < send_data_len; i++) {
        printf("%02X ", send_data[i]);
    }
    printf("\n");

}

/******************************************************************************
* @brief Encrypt a plain text string to MD5 
*   
* @param[in] str    - string buffer
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
