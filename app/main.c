#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include "http_resp.h"
#include "misc.h"
#include <wiringPiI2C.h>

#define I2C_SLAVE_ADDR  0x08


int i2c_fd = -1;

int 
main ()
{
    // init i2c
    i2c_fd = wiringPiI2CSetup(I2C_SLAVE_ADDR);
    if (i2c_fd < 0) {
        printf("cannot open i2c\n");
    }
    struct MHD_Daemon *daemon;
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
 //   getchar ();
    while(1); 
    MHD_stop_daemon (daemon);
    printf("http server stopped\n");
    return 0;
}
