#include "gps.h"
#include <string.h>
#include <stdio.h>



#define RECV_BUFF_SIZE                 1024
/*****************************************************************************
* LOCAL VARIABLES
*/
static unsigned int l_latitude;         /* latitude of drone 's location */ 
static unsigned int l_longitude;        /* longitude of drone 's location */
static unsigned int l_speed;            /* speed of drone */
static unsigned int buff_idx = 0;       /* start at beginning */
static char         recv_buff[RECV_BUFF_SIZE] = {0};

static unsigned int
parse_decimal(char *str) {
  unsigned int d = 0;

  while (str[0] != 0) {
    if ((str[0] > '9') || (str[0] < '0'))
      return d;
    d *= 10;
    d += str[0] - '0';
    str++;
  }
  return d;
}

/******************************************************************************
* @brief Parse data from serial data line RMC
* @param[in]  inp_buff  - input buffer  
* @param[in]  len       - length of data input buffer    
* @return   None
*/
// void serial_parse_data_rmc(char * inp_buff, int len) 
// {
//   unsigned int  latitude;
//   unsigned int  longitude;
//   unsigned char ground_speed, track_angle;
//   char          lat_dir, long_dir;  
//   char *        parse_ptr;
//   if (NULL == inp_buff || 0 > len) {
//     return;
//   }     
//   parse_ptr = inp_buff + 7;
//   parse_ptr = strchr(parse_ptr, ',') + 1;
//   parse_ptr += 2;
//   // grab latitude & longitude data
//   //latitude
//    latitude = parse_decimal(parse_ptr);
//    if (0 != latitude) {
//      latitude *= 10000;
//      parse_ptr = strchr(parse_ptr, '.') + 1;
//      latitude += parse_decimal(parse_ptr);
//    }
//    parse_ptr = strchr(parse_ptr, ',') + 1;
//    // read latitude N/S data
//    if (parse_ptr[0] != ',') {
//      lat_dir = parse_ptr[0];
//    }
  
//    //Serial.println(latdir);
  
//    // longitude
//    parse_ptr = strchr(parse_ptr, ',') + 1;
//    longitude = parse_decimal(parse_ptr);
//    if (0 != longitude) {
//      longitude *= 10000;
//      parse_ptr = strchr(parse_ptr, '.') + 1;
//      longitude += parse_decimal(parse_ptr);
//    }
//    parse_ptr = strchr(parse_ptr, ',') + 1;
//    // read longitude E/W data
//    if (parse_ptr[0] != ',') {
//      long_dir = parse_ptr[0];
//    }
//    // groundspeed
//    parse_ptr = strchr(parse_ptr, ',')+1;
//    ground_speed = parse_decimal(parse_ptr);

//    // track angle
//    parse_ptr = strchr(parse_ptr, ',')+1;
//    track_angle = parse_decimal(parse_ptr);

//    l_longitude = longitude;
//    l_latitude = latitude;
//    l_speed = ground_speed;
// }

/******************************************************************************
* @brief Get GPS information
* @param[out]  latitude  - latitude of drone  
* @param[out]  longitude - longitude of drone
* @param[out]  speed     - speed of  drone
*/
int get_gps_data(unsigned int * latitude,
        unsigned int * longitude,
        unsigned int * speed 
) 
{
  *latitude = l_latitude;
  *longitude = l_longitude;
  *speed = l_speed;
  return 0;
}

/**
*
*/
char * 
gps_parser(char * rx_buffer, int len) 
{
  char * p_start_rmc = NULL;
  p_start_rmc = strstr((const char *)rx_buffer, "$GNRMC"); 
  if (NULL == p_start_rmc) {
    printf("Can not find RMC line start\n");
    return NULL;
  }
  return strtok(p_start_rmc, "*");
}
