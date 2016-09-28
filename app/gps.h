#ifndef __GPS_H_
#define __GPS_H_


/******************************************************************************
* @brief Get GPS information
* @paran[out]  latitude  - latitude of drone  
* @paran[out]  longitude - longitude of drone
* @paran[out]  speed     - speed of  drone
*/
int get_gps_data(unsigned int * latitude,
        unsigned int * longitude,
        unsigned int * speed
);

#endif  /* End of __GPS_H_ */
