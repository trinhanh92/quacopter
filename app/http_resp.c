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

/******************************************************************************
*
*
*
*
*/
static int
print_out_key (void *cls, enum MHD_ValueKind kind, const char *key,
const char *value)
{
    printf ("%s: %s\n", key, value);
    return MHD_YES;
}

/******************************************************************************
*
*
*
*
*/
static int
send_resp(struct MHD_Connection *connection,
            char* resp, int status)
{
    int ret;
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer (strlen (resp), resp,
                    MHD_RESPMEM_MUST_COPY);
    // response = MHD_create_response_from_buffer (2, (void*) resp,
    //                 MHD_RESPMEM_PERSISTENT);
    if (!response) 
        return MHD_NO;
    ret = MHD_queue_response (connection, status, response);
    MHD_destroy_response (response);
    return ret;
}

/******************************************************************************
*
*
*
*
*/
static int
process_post_data(const char *url,char *buffer, int buffer_len, char *resp)
{
    int ret_val;
    char x_axis[10] = {0};
    char y_axis[10] = {0};
    char z_axis[10] = {0};
    char sig_recv[50] = {0};
    char *sig_created;
    char raw_data[50] = {0};

    if (0 == strcmp(url, "/api/move")) {            // case 1.2
        // continue process
    } else if (0 == strcmp(url, "/api/info")) {      // case 1.1
        // strcpy(resp, "No support\r\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, NO_SUPPORT, "null");
        return MHD_YES;
    } else {                                        // unsupport command
        strcpy(resp, NOT_FOUND);
        return MHD_NO;
    }
    // parse json data from post request
    memset(x_axis, 0, sizeof x_axis);
    memset(y_axis, 0, sizeof y_axis);
    memset(z_axis, 0, sizeof z_axis);
    memset(sig_recv, 0, sizeof sig_recv);
    ret_val = json_parser(buffer, buffer_len, "x", x_axis);
    ret_val += json_parser(buffer, buffer_len, "y", y_axis);
    ret_val += json_parser(buffer, buffer_len, "z", z_axis);
    ret_val += json_parser(buffer, buffer_len, "sig", sig_recv);
    if (ret_val < 0) {
        printf("[POST] - Request params invalid\n");
        //TODO: Handle params invalid
        // strcpy(resp, "Invalid params\r\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_PARAMS, "null");
        return MHD_YES;
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
        // strcpy(resp, "Invalid signature\r\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_SIGNATURE, "null");
        //TODO: Handle invalid signature 
        return MHD_YES;
    }

    snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, SUCCESS, "null");
    // printf("Response: %s\n", resp);
    return MHD_YES;
}

/******************************************************************************
*
*
*
*
*/
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
                        size_t *upload_data_size, void **con_cls)
{
    int  ret_val;
    int  status_code;
    char resp_data[MAX_RESP_BUFF_SIZE] = {0};


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
        // con_data->is_parsed = false;
        con_data->recv_data = NULL;
        if (0 == strcmp (method, "POST")) {
            con_data->connectiontype = POST;
        } else {
            con_data->connectiontype = GET;
        }
        *con_cls = (void *) con_data;
        return MHD_YES;
    }
    if (0 == strcmp(method, "POST")) {
       struct connection_data_s *con_data = *con_cls;

        // *upload_data_size = zere mean all data have been received
        if (*upload_data_size != 0) {
            printf("post data available\n");
            status_code = MHD_HTTP_OK;
            // printf("data receive:  %.*s\n", *upload_data_size, upload_data);

            con_data->recv_data = (char *)upload_data;
            con_data->recv_data_len = *upload_data_size; 
            *upload_data_size = 0;

            strcpy(resp_data, "hello post\n");
            return MHD_YES;
        } else {
            // handle post response here
            printf("[POST] - Data: %.*s\n", con_data->recv_data_len, con_data->recv_data);
            ret_val = process_post_data(url, con_data->recv_data, con_data->recv_data_len, resp_data);
            if(MHD_YES == ret_val) {
                status_code = MHD_HTTP_OK;
            } else {
                status_code = MHD_HTTP_NOT_FOUND;
            }
            printf("Response POST request: %.*s\n", strlen(resp_data), resp_data);
            return send_resp (connection, resp_data, status_code);
        }
    } else {
        status_code = MHD_HTTP_NOT_FOUND;
        return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);
    }
    return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);

}