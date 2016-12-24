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
#include <stdlib.h>
#include "common.h"

#define TIMEOUT_FOR_DETECTION         (30000)

int i2c_fd = -1;

int 
main ()
{
    // init i2c
    unsigned char curr_mode, last_mode;
    int retry_detect = 0;
    int redirect = 0;

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
    // getchar();
    // char usr_inp = (unsigned char) getchar ();
    // if (usr_inp == 'A') {
    //    curr_mode = MODE_AUTO;
    // } else {
    //    curr_mode = MODE_MANUAL;
    // }
    // set_mode(curr_mode);
    // switch mode for the car
    while(1) {
        //TODO: check PIR and Light sensor here to turn light ON
        //TODO: get current mode
        get_mode(&curr_mode);
        if (curr_mode != MODE_AUTO || curr_mode != MODE_MANUAL) {
            set_mode(MODE_AUTO & 0xFF);
            get_mode(&curr_mode);
        }
        switch (curr_mode) {
            case MODE_AUTO:
            // execute auto mode here - people detection
            // if(detected) {
            //     // go ahead
            // }
                // if (last_mode == curr_mode) {
                //     printf("still auto mode\n");
                //     continue;
                // }
                last_mode = curr_mode;
                printf("auto mode\n");
                system("sudo pkill uv4l");
                if (1 == system("./people_detection")) {     // peeple detected
                    retry_detect = 0;                          // reset retry counter
                    redirect = ~redirect;
                    wiringPiI2CWrite(i2c_fd, ROTATE_STOP);     // stop the servo 
                    wiringPiI2CWrite(i2c_fd, GO_FORWARD);     // go ahea
                } else {                                // not detected
                    retry_detect++;
                    if(TIMEOUT_FOR_DETECTION <= retry_detect) {
                        retry_detect = 0;                // reset detect fail counter
                        if (0 == redirect) {
                            wiringPiI2CWrite(i2c_fd, ROTATE_LEFT);     // rotate servo left 
                        } else {
                            wiringPiI2CWrite(i2c_fd, ROTATE_RIGHT);    // rotate servo left 
                        }
                        redirect = !redirect;
                    }
                }
                // mode_done = 0;
            break;
            case MODE_MANUAL:
                if (last_mode == curr_mode) {
                    printf("still manual mode\n");
                    continue;
                }
                last_mode = curr_mode;
            // execute auto mode here - stream video to phone
                printf("manual mode\n");
                // system(UV4L_STREAMING);
            break;
            default:
            // printf("default mode\n");
            break;
        }
        usleep(1);  // sleep 1us
    }
    MHD_stop_daemon (daemon);
    printf("http server stopped\n");
    return 0;
}
