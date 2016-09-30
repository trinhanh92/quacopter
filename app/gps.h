#ifndef __GPS_H_
#define __GPS_H_


/**
* @brief parse data rmc
*/
void gps_parse_data_rmc(char * rmc_buff, int len);

char * 
gps_parser(char * rx_buffer, int len);
// char *
// rmc_shift_ptr(char * src_buff, char * token, int times_to_shift);

/******************************************************************************
* @brief Get GPS information
* @paran[out]  latitude  - latitude of drone  
* @paran[out]  longitude - longitude of drone
* @paran[out]  speed     - speed of  drone
*/
int get_gps_data(unsigned int * latitude,
        unsigned int * longitude
);

#endif  /* End of __GPS_H_ */
