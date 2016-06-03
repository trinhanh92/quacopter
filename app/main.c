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

    // initialize spi master
    spi_fd = wiringPiSPISetup(SPI_CS_CHANNEL, SPI_CLK);

    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                    &http_resp_handler, NULL,
                    MHD_OPTION_NOTIFY_COMPLETED, &request_completed,
                    NULL, MHD_OPTION_END);

    // daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
    //                      &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        printf("http start failure\n");
        return 1;
    }
    printf("http server started\n");
    getchar ();
    MHD_stop_daemon (daemon);
    printf("http server stopped\n");
return 0;
}
