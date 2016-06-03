#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "http_resp.h"
#include "misc.h"
#include "microhttpd.h"

/******************************************************************************
* @brief This function used to print request header
* 
* @param[in] cls
* @param[in] kind
* @param[in] key
* @param[in] value
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
* @brief This function used to send response to client
* 
* @param[in]    connection - connection handle 
* @param[in]    resp       - data buffer to send
* @param[in]    status     - http status code to response
*
* @return       NHD_YES if success, NHD_NO for failure
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
    if (MHD_HTTP_OK == status) {
        MHD_add_response_header(response, "Content-Type", "application/json");
    } else {
        MHD_add_response_header(response, "Content-Type", "text/html");
    }
    ret = MHD_queue_response (connection, status, response);
    MHD_destroy_response (response);
    return ret;
}

/******************************************************************************
* @brief This function used to process request data 
* 
* @param[in]    url          - request command 
* @param[in]    buffer       - request data 
* @param[in]    buffer_len   - request data length
* @param[in]    resp         - response data
*
* @return       http status code
*
*/
static int
process_post_data(const char *url,char *buffer, int buffer_len, char *resp)
{
    int ret_val;
    char x_val[4] = {0}; 
    char y_val[4] = {0}; 
    char z_val[4] = {0}; 
    char sig_recv[33] = {0};

    char *sig_created;
    char raw_data[50] = {0};
    req_data_t req_data;

    if (0 == strcmp(url, CMD_DEV_CTRL)) {            // case 1.2
        // continue process
    } else if (0 == strcmp(url, CMD_DEV_INFO)) {      // case 1.1
        // strcpy(resp, "No support\r\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, NO_SUPPORT, "null");
        return MHD_HTTP_OK;
    } else {                                        // unsupport command
        strcpy(resp, NOT_FOUND);
        return MHD_HTTP_NOT_FOUND;
    }

    // case post data null - response bad request
    if(0 == buffer_len) {
        strcpy(resp, BAD_REQUEST);
        return MHD_HTTP_BAD_REQUEST;     
    } 
    // parse data from post request

    // find x value
    ret_val = parse_request(buffer, buffer_len, "x", x_val);
    if (ret_val < 0) {
        printf("[POST] - Request param X invalid\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_PARAMS, "null");
        return MHD_HTTP_OK;
    }
    // find y value
    ret_val = parse_request(buffer, buffer_len, "y", y_val);
    if (ret_val < 0) {
        printf("[POST] - Request param Y invalid\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_PARAMS, "null");
        return MHD_HTTP_OK;
    }

    // find z value
    ret_val = parse_request(buffer, buffer_len, "z", z_val);
    if (ret_val < 0) {
        printf("[POST] - Request param Z invalid\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_PARAMS, "null");
        return MHD_HTTP_OK;
    }

    // find sig value
    ret_val = parse_request(buffer, buffer_len, "sig", sig_recv);
    if (ret_val < 0) {
        printf("[POST] - Request param SIG invalid\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_PARAMS, "null");
        return MHD_HTTP_OK;
    }

    // show data
    req_data.x = atoi(x_val);
    req_data.y = atoi(y_val);
    req_data.z = atoi(z_val);
    strncpy(req_data.sig, sig_recv, sizeof sig_recv);

    printf("x: %d\n", req_data.x);
    printf("y: %d\n", req_data.y);
    printf("z: %d\n", req_data.z);
    printf("sig: %s\n", req_data.sig);
    // compare signature
    snprintf(raw_data, sizeof (raw_data), "%d%d%d%s", req_data.x,
                         req_data.y, req_data.z, SERCRET_KEY);
    sig_created = str2md5(raw_data, strlen (raw_data));
    printf("signature created: %s\n", sig_created);
    if (0 != strcmp(sig_created, req_data.sig)) {
        // invalid signature
        printf("[POST] - Invalid signature\n");
        // strcpy(resp, "Invalid signature\r\n");
        snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, INVALID_SIGNATURE, "null");
        //TODO: Handle invalid signature 
        return MHD_HTTP_OK;
    }

    snprintf(resp, MAX_RESP_BUFF_SIZE, RESP_DATA_FORMAT, SUCCESS, "null");
    // printf("Response: %s\n", resp);
    return MHD_HTTP_OK;
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
            return MHD_YES;
        } else {
            // handle post response here
            printf("[POST] - Data: %.*s\n", con_data->recv_data_len, con_data->recv_data);
            status_code = process_post_data(url, con_data->recv_data, con_data->recv_data_len, resp_data);
            printf("Response POST request: %.*s\n", strlen(resp_data), resp_data);
            return send_resp (connection, resp_data, status_code);
        }
    } else { 
        /* No support GET method*/
        return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);
    }
    return send_resp (connection, NOT_FOUND, MHD_HTTP_NOT_FOUND);

}
