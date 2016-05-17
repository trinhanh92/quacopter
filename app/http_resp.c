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
    int  status_code;
    char x_axis[10] = {0};
    char y_axis[10] = {0};
    char z_axis[10] = {0};
    char sig_recv[50] = {0};
    char *sig_created;
    char raw_data[50] = {0};
    char resp_data[MAX_RESP_BUFF_SIZE] = {0};
    char output[MAX_RESP_BUFF_SIZE] = {0};

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
                // parse json data from post request
                memset(x_axis, 0, sizeof x_axis);
                memset(y_axis, 0, sizeof y_axis);
                memset(z_axis, 0, sizeof z_axis);
                memset(sig_recv, 0, sizeof sig_recv);
                json_parser(upload_data, *upload_data_size, "x", x_axis);
                json_parser(upload_data, *upload_data_size, "y", y_axis);
                json_parser(upload_data, *upload_data_size, "z", z_axis);
                json_parser(upload_data, *upload_data_size, "sig", sig_recv);
                *upload_data_size = 0;

                if ((NULL == x_axis) || (NULL == y_axis) || 
                    (NULL == z_axis) || (NULL == sig_recv)) {
                    printf("[POST] - Request params invalid\n");
                    return MHD_YES;
                    //TODO: Handle params invalid
                    strcpy(resp_data, "Invalid params\r\n");
                }
                printf("x: %s\n", x_axis);
                printf("y: %s\n", y_axis);
                printf("z: %s\n", z_axis);
                printf("sig: %s\n", sig_recv);
                // compare signature
                snprintf(raw_data, sizeof (raw_data), "%s%s%s%s", x_axis, y_axis, z_axis, SERCRET_KEY);
                sig_created = str2md5(raw_data, strlen (raw_data));
                printf("signature created: %s\n", sig_created);
                if (0 != strcmp(sig_created, sig_recv)) {
                    // invalid signature
                    printf("[POST] - Invalid signature\n");
                    strcpy(resp_data, "Invalid signature\r\n");
                    //TODO: Handle invalid signature 
                    return MHD_YES;
                }
                memcpy(con_data->resp_data,resp_data, strlen (resp_data));
                return MHD_YES;
            } else {
                // handle post response here
                memcpy(resp_data, con_data->resp_data, strlen (con_data->resp_data));
                printf("Response POST request %d\n", strlen(resp_data));
                // free(con_data);
                return send_resp (connection, resp_data, MHD_HTTP_OK);
            }
       }
    } else {
        status_code = MHD_HTTP_NOT_FOUND;
        return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);
    }
    return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);

    // int http_code;
    // const char *content_type = "text/plain";
    // if (strcmp(method, MHD_HTTP_METHOD_POST) == 0)
    //     {
    //             struct connection_data_s* connection_data = NULL;
               
    //             connection_data = (*con_cls);
    //             if (NULL == connection_data)
    //             {
    //                     connection_data = malloc(sizeof (struct connection_data_s));
    //                     connection_data->is_parsed = false;
    //                     *con_cls = connection_data;
    //             }
               
    //             if (!connection_data->is_parsed)
    //             {
    //                     // First this method gets called with *upload_data_size == 0
    //                     // just to let us know that we need to start receiving POST data
    //                     connection_data->is_parsed = true;
    //                     return MHD_YES;
    //             }
    //             else
    //             {
    //                     if (*upload_data_size != 0)
    //                     {
    //                             // Receive the post data and write them into the bufffer
    //                             strncpy(connection_data->recv_data, upload_data, *upload_data_size);
    //                             *upload_data_size = 0;
    //                             return MHD_YES;
    //                     }
    //                     else
    //                     {
    //                             // *upload_data_size == 0 so all data have been received
    //                             strcpy(output, "Received data:\n\n");
    //                             // strcat(output, connection_data->recv_data);
                               
    //                             free(connection_data);
    //                             *con_cls = NULL; 
    //                     }
    //             }
    //     }
    //     else
    //     {
    //             http_code = MHD_HTTP_NOT_FOUND;
    //             content_type = "text/plain";
    //             strcpy(output,"Unknown request method");
    //     }
 
    //     const char* output_const = output;
 
    //     struct MHD_Response *response = MHD_create_response_from_buffer(
    //                                     strlen(output_const), (void*)output_const, MHD_RESPMEM_MUST_COPY);
 
    //     MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, content_type);
 
    //     int ret = MHD_queue_response(connection, http_code, response);
 
    //     MHD_destroy_response(response);
       
    //     return ret;
}