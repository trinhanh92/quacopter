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
#include <unistd.h>

unsigned char * p_start_rmc = NULL;
latlng_t g_lat_val;
latlng_t g_lng_val;
volatile int g_role = 0;
u8_t rf_data[33] = {0};
pthread_mutex_t lock;
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
    // u8_t spi_data[6];
    // uint8_t recv_buff[100] = {0};
    // int recv_len = 0;
    // req_data_t req_data;
   
   // init RF24
    rf_init();

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    ret_val = pthread_create (&rf_thread, NULL, rf_thread_func, NULL);
    if (ret_val) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", ret_val);
        exit(EXIT_FAILURE);
    }
    // initialize spi master
    // spi_fd = wiringPiSPISetup(SPI_CS_CHANNEL, SPI_CLK);

    //start server
    daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, 
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
    MHD_stop_daemon (daemon);
    pthread_mutex_destroy(&lock);
    printf("http server stopped\n");
    return 0;
}

void *rf_thread_func(void * ptr)
{
  uint8_t recv_buff[100] = {0};
  unsigned int recv_len;
  // latlng_t lat_val, lng_val; 
 // g_lat_val.val = 10.87135;
 // g_lng_val.val = 106.79974; 

  while (1) {
    if (g_role == 0) {
		// pthread_mutex_lock(&lock);
		// g_lat_val.val = 10.87135;
		// g_lng_val.val = 106.79974;
		pthread_mutex_unlock(&lock); 
        // rf_start_listenning();
        while(rf_data_available()) {
            recv_len = nrf24_getDynamicPayloadSize();
            nrf24_read(&recv_buff, recv_len);
            printf("receiving data: %s\n", recv_buff);
			pthread_mutex_lock(&lock);
            memcpy(g_lat_val.val_in_bytes, &recv_buff[1], 4);
            memcpy(g_lng_val.val_in_bytes, &recv_buff[5], 4);
			pthread_mutex_unlock(&lock); 
        }
    } else {
        rf_stop_listenning();
        rf_send_data(rf_data, 32);
        g_role = 0;
    }
    usleep(1);
  }
}

