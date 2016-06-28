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

int 
main ()
{
    struct MHD_Daemon *daemon;
    int spi_fd;                   // spi file description
    u8_t spi_data[6];
    req_data_t req_data;

    // initialize spi master
    spi_fd = wiringPiSPISetup(SPI_CS_CHANNEL, SPI_CLK);

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
