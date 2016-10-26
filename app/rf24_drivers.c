#include "rf24_drivers.h"
#include <RF24/nrf24.h>
#include <RF24/bcm2835.h>
#include <string.h>

const uint8_t addresses_trans[] =  {0x05, 0x04, 0x03, 0x02, 0x01};
const uint8_t addresses_recv[]  = {0x01, 0x02, 0x03, 0x04, 0x05};
 
void rf_init ()
{
	nrf24_init(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_1MHZ);
	nrf24_begin();
	nrf24_enableDynamicPayloads();
	nrf24_setAutoAck(1);
	nrf24_setRetries(15,15);
	nrf24_setDataRate(RF24_1MBPS);
	nrf24_setPALevel(RF24_PA_MAX);
	nrf24_setChannel(0x00);
	nrf24_setCRCLength(RF24_CRC_16);
	nrf24_openWritingPipe(addresses_trans);
	nrf24_openReadingPipe(1,addresses_recv);
	nrf24_printDetails();
	nrf24_startListening();
}

void rf_stop_listenning() 
{
	nrf24_stopListening();
}

void rf_start_listenning() 
{
	nrf24_startListening();
}

bool rf_data_available()
{
	return nrf24_available(NULL);
}

void rf_send_data(uint8_t * out_buff, uint32_t len)
{
	nrf24_send(out_buff, len);
}

void rf_recv_data(uint8_t * in_buff, uint32_t len)
{
	nrf24_read(&in_buff, len);
}