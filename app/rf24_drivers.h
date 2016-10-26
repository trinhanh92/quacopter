#ifndef __RF24_DRIVERS_H_
#define __RF24_DRIVERS_H_

#include <RF24/nrf24.h>

void rf_init();
bool rf_data_available(void);
void rf_send_data(uint8_t * out_buff, uint32_t len);
void rf_recv_data(uint8_t * in_buff, uint32_t len);
void rf_stop_listenning(); 
void rf_start_listenning(); 

#endif
