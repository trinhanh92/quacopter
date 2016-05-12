#ifndef __HTTP_RESP_H_
#define __HTTP_RESP_H_

#include "microhttpd.h"

#define MAX_RESP_BUFF_SIZE   5*1024
#define RESP_DATA_FORMAT     "{\r\n\"error\": %d,\r\n\"data\": %s\r\n}\r\n"

// HTTP method
#define GET                  0
#define POST                 1

#define POST_BUFFER_SIZE     512
#define MAX_NAME_SIZE        20
#define MAX_ANSWER_SIZE      512
// SERVER_RESP_ERROR_CODE
#define SUCCESS              0
#define INVALID_PARAMS      -1
#define INVALiD_SIGNATURE   -2
#define NO_SUPPORT          -3

// 
struct connection_info_struct
{
    int connectiontype;
    char *answerstring;
    struct MHD_PostProcessor *postprocessor;
};

void
request_completed (void *cls, struct MHD_Connection *connection,
                        void **con_cls, enum MHD_RequestTerminationCode toe);
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls);
#endif
