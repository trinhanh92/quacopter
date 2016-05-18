#ifndef __HTTP_RESP_H_
#define __HTTP_RESP_H_

#include <stdbool.h>
#include "microhttpd.h"

#define PORT 8080

#define RESP_DATA_FORMAT     "{\"error\": %d,\"data\": %s}"
#define RESP_DEV_INFO_FORMAT "{\"Device_info\":%s,\"Battery_info\":%s,\"Network_info\":%s}"
// HTTP method
#define GET                  0
#define POST                 1

#define MAX_RESP_BUFF_SIZE   1024
#define POST_BUFFER_SIZE     512
#define MAX_NAME_SIZE        20
#define MAX_ANSWER_SIZE      512
// SERVER_RESP_ERROR_CODE
#define SUCCESS              0
#define INVALID_PARAMS      -1
#define INVALID_SIGNATURE   -2
#define NO_SUPPORT          -3
#define NOT_FOUND			"<html><body>404 Not found.</body></html>"

// define command
#define CMD_DEV_INFO		"/api/info"
#define CMD_DEV_CTRL		"/api/move"

struct connection_data_s {
	bool is_parsed;
	int  connectiontype;
	char *recv_data;
	int  recv_data_len;
};

/******************************************************************************
*
*
*
*
*/
void
request_completed (void *cls, struct MHD_Connection *connection,
                        void **con_cls, enum MHD_RequestTerminationCode toe);

/******************************************************************************
*
*
*
*
*/
int
http_resp_handler (void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls);
#endif
