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
        unsigned int * longitude
) 
{
  *latitude = l_latitude;
  *longitude = l_longitude;
  return 0;
}


/**
*
*/
char *
rmc_shift_ptr(char * src_buff, char * token, int times_to_shift)
{
  char * ptr = src_buff; 
  int cnt;
  for (cnt = 0; cnt < times_to_shift; cnt++) {
    // printf("time %d\n", cnt + 1);
    ptr = strstr(ptr, token);
    ptr++; 
  } 
  return ptr;
}

/**
* @brief parse data rmc
*/
void gps_parse_data_rmc(char * rmc_buff, int len) 
{
  char * parse_ptr;
  char * long_ptr = NULL;
  char * lat_ptr = NULL;
  long_ptr =  rmc_shift_ptr (rmc_buff, ",", 3);
  if (NULL != long_ptr) {
    lat_ptr  =  rmc_shift_ptr (long_ptr, ",", 2);
    // printf("long ptr %s\n", long_ptr);
    // printf("lat ptr %s\n", lat_ptr);
  } else {
    printf("can not find longitude data\n");
    return;
  }
  if (NULL != lat_ptr) {
    // get longitude
    parse_ptr = long_ptr;
    l_longitude = parse_decimal (parse_ptr);
    l_longitude *= 100000;
    parse_ptr = strstr(parse_ptr, ".") + 1;
    l_longitude += parse_decimal (parse_ptr);
    //get latitude
    parse_ptr = lat_ptr;
    l_latitude = parse_decimal (parse_ptr);
    l_latitude *= 100000;
    parse_ptr = strstr(parse_ptr, ".") + 1;
    l_latitude += parse_decimal (parse_ptr);
    printf("long = %d, lat = %d\n", l_longitude, l_latitude);
  } else {
    printf("can not find latitude data\n");
    return;
  }
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
