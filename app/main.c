#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include "http_resp.h"
#include "misc.h"
#include <wiringPiSPI.h>
#include "rf24_drivers.h"
#include <pthread.h>

unsigned char * p_start_rmc = NULL;
volatile latlng_t g_lat_val;
volatile latlng_t g_lng_val;
/**
* @brief function handle gps process in gps thread
*/
void *rf_thread_func(void *ptr);

int 
main ()
{
    pthread_t  rf_thread; 
    struct MHD_Daemon *daemon;
    int ret_val;
    // int spi_fd;                   // spi file description
    u8_t spi_data[6];
    req_data_t req_data;
   
   // init RF24
    rf_init();

    ret_val = pthread_create (&rf_thread, NULL, rf_thread_func, NULL);
    if (ret_val) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", ret_val);
        exit(EXIT_FAILURE);
    }
    // initialize spi master
    // spi_fd = wiringPiSPISetup(SPI_CS_CHANNEL, SPI_CLK);

    //start server
    daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG, PORT, NULL, 
                    NULL, &http_resp_handler, NULL,
                    MHD_OPTION_NOTIFY_COMPLETED, &request_completed,
                    NULL,
                    MHD_OPTION_END);

    // daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
    //                      &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        printf("http start failure\n");
        return 1;
    }
    printf("http server started\n");
    getchar ();
    // request to quadcopter to land
    // package data to spi slave
    // req_data.x = 0;
    // req_data.y = 0;
    // req_data.z = 0;
    // spi_data_to_send(req_data, spi_data, sizeof spi_data);
    // rf_stop_listenning();
    // rf_send_data(spi_data, sizeof spi_data);
    // rf_start_listenning();
    // //TODO: send data RF
    // wiringPiSPIDataRW(SPI_CS_CHANNEL, spi_data, 2);
    // wiringPiSPIDataRW(SPI_CS_CHANNEL, &spi_data[2], 2);
    // wiringPiSPIDataRW(SPI_CS_CHANNEL, &spi_data[4] , 2);
    MHD_stop_daemon (daemon);
    printf("http server stopped\n");
    return 0;
}

void *rf_thread_func(void * ptr)
{
  uint8_t recv_buff[100] = {0};
  // latlng_t lat_val, lng_val;
  while (1) {
    while(rf_data_available()) {
        nrf24_read(&recv_buff, sizeof recv_buff);
        printf("receiving data: %s\n", recv_buff);
        memcpy(g_lat_val.val_in_bytes, &recv_buff[1], 4);
        memcpy(g_lng_val.val_in_bytes, &recv_buff[5], 4);
    }
    sleep(1);
  }
}

