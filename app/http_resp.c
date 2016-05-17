#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "http_resp.h"
#include "misc.h"
#include "microhttpd.h"
#include <openssl/md5.h>
#include "jsmn.h"

static int
print_out_key (void *cls, enum MHD_ValueKind kind, const char *key,
const char *value)
{
    printf ("%s: %s\n", key, value);
    return MHD_YES;
}

static int
send_resp(struct MHD_Connection *connection,
const char* resp, int status)
{
    int ret;
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer (strlen (resp), (void*) resp,
                    MHD_RESPMEM_PERSISTENT);
    if (!response) 
        return MHD_NO;
    ret = MHD_queue_response (connection, status, response);
    MHD_destroy_response (response);
    return ret;
}

void
request_completed (void *cls, struct MHD_Connection *connection,
                        void **con_cls, enum MHD_RequestTerminationCode toe)
{
    struct connection_data_s *con_info = *con_cls;
    if (NULL == con_info)
        return;
    if (con_info->connectiontype == POST) {
        printf("post data completed\n");
        // free (con_info->receive_data);
        // send_resp (connection, "hello wol", MHD_HTTP_OK);
    } else {
        printf("get data completed\n");
    }
    free (con_info);
    *con_cls = NULL;
}

int
answer_to_connection (void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls)
{
    int status_code;
    if (NULL == *con_cls) {
        printf("post parse header\n");
        printf ("New %s request for %s using version %s\n", method, url, version);
        MHD_get_connection_values (connection, MHD_HEADER_KIND, print_out_key,
                                     NULL);
        struct connection_data_s *con_data;
        con_data = malloc (sizeof (struct connection_data_s));
        if (NULL == con_data) {
            return MHD_NO;
        }
        con_data->is_parsed = false;
        if (0 == strcmp (method, "POST")) {
            con_data->connectiontype = POST;
        } else {
            con_data->connectiontype = GET;
        }
        *con_cls = (void *) con_data;
        // return MHD_YES;
    }
    if (0 == strcmp(method, "POST")) {
       struct connection_data_s *con_data = *con_cls;
       if(false == con_data->is_parsed) {
            printf("data is not parse yet!\n");
            con_data->is_parsed = true;
            return MHD_YES;
       } else {
        // *upload_data_size = zere mean all data have been received
            if (*upload_data_size != 0) {
                printf("post data available\n");
                status_code = MHD_HTTP_OK;
                printf("data receive:  %.*s\n", *upload_data_size, upload_data);
                *upload_data_size = 0;
                return MHD_YES;
            } else {
                return send_resp (connection, "hello", MHD_HTTP_OK);
            }
       }
    } else {
        status_code = MHD_HTTP_NOT_FOUND;
        return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);
    }
    return send_resp (connection, NOT_FOUND, MHD_HTTP_OK);
}