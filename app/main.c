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
#include "serial.h"
#include <pthread.h>

unsigned char * p_start_rmc = NULL;
/**
* @brief function handle gps process in gps thread
*/
void *gps_thread_func(void *ptr);

int 
main ()
{
    pthread_t  gps_thread; 
    int        ret_val;
    struct MHD_Daemon *daemon;
    int spi_fd = -1;                   // spi file description
    u8_t spi_data[6];
    req_data_t req_data;

    ret_val = pthread_create (&gps_thread, NULL, gps_thread_func, NULL);
    if (ret_val) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", ret_val);
        exit(EXIT_FAILURE);
    }
    // initialize spi master
    spi_fd = wiringPiSPISetup(SPI_CS_CHANNEL, SPI_CLK);
    if (spi_fd < 0) {
        printf("spi init error\n");
        return 1;
    }

    // create thread for reading serial port
    
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
    req_data.x = 0;
    req_data.y = 0;
    req_data.z = 0;
    spi_data_to_send(req_data, spi_data, sizeof spi_data);
    // send data
    wiringPiSPIDataRW(SPI_CS_CHANNEL, spi_data, 2);
    wiringPiSPIDataRW(SPI_CS_CHANNEL, &spi_data[2], 2);
    wiringPiSPIDataRW(SPI_CS_CHANNEL, &spi_data[4] , 2);
    MHD_stop_daemon (daemon);
    printf("http server stopped\n");
    return 0;
}

void *gps_thread_func(void * ptr)
{
  int uart_fd = -1;
  int rx_length = -1; 
  unsigned char rx_buffer[UART_RECEIVE_BUFF_SIZE] = {0};   
  // init serial port
  uart_fd = uart_open();
  if (uart_fd < 0) {
    return 1;
  }

  while (1) {
    rx_length = uart_read (uart_fd, rx_buffer, sizeof rx_buffer);
    if (0 < rx_length) {
      p_start_rmc = gps_parser(rx_buffer, rx_length);
      gps_parse_data_rmc(p_start_rmc, strlen(p_start_rmc));
      // printf("cnt: %d\n%s\n", cnt, p_start_rmc);
    }
    sleep(1);
  }
}
