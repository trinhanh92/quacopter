#ifndef __MISC_H__
#define __MISC_H_

#define SERCRET_KEY "drone_hung_hue@uit.edu.vn"


#define  SPI_CS_CHANNEL  1      // choose chip select pin (CE1)
#define  SPI_CLK         500000 // set spi master clock      


typedef unsigned short  u16_t;
typedef signed short    i16_t;
typedef unsigned char   u8_t;
typedef signed char     i8_t;

/** location data */
typedef union latlng_u {
	float val;
	u8_t   val_in_bytes[4];
} latlng_t;

/* data from client request */
typedef struct req_data_s {
	u8_t msg_type;
	i8_t x;				// value of x
	i8_t y;				// value of y
	i8_t z;				// value of z
	u8_t t;				// value of t
	u8_t m;				// value of m
	latlng_t lat;				// value of lat 
	latlng_t lng;				// value of lng 
	char sig[33];		// signature in MD5
} req_data_t;


/******************************************************************************
* @brief Convert 2 byte data to single byte array
*   
* @param[in]  inp_num    - input number
* @param[out] byte_arr   - output array
* @return     None
*/
void
short2byte(i16_t inp_num, u8_t *byte_arr);

/******************************************************************************
* @brief Package data to send to SPI slave
*   
* @param[in]  req_data      - request data
* @param[out] send_data     - send data buffer in hex
* @param[in]  send_data_len - length of send data in bytes
* @return     None
*/
void
spi_data_to_send(req_data_t req_data, u8_t *send_data, int send_data_len);


/******************************************************************************
* @brief Package data to send to RF 
*   
* @param[in]  req_data      - request data
* @param[out] send_data     - send data buffer in hex
* @param[in]  send_data_len - length of send data in bytes
* @return     None
*/
void
rf_data_to_send(req_data_t req_data, u8_t *send_data, int send_data_len);
/******************************************************************************
* @brief Encrypt a plain text o MD5 
*   
* @param[in] str    - string buffer input
* @param[in] length - string buffer length
* @return    MD5 string
*/
char 
*str2md5(const char *str, int length);

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
parse_request(char *request, int req_len, char *key, char *value);
#endif
