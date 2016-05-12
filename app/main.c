#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include "http_resp.h"
#include "misc.h"

#define PORT 8888
// static int
// print_out_key (void *cls, enum MHD_ValueKind kind, const char *key,
// const char *value)
// {
//     printf ("%s: %s\n", key, value);
//     return MHD_YES;
// }

// static int
// answer_to_connection (void *cls, struct MHD_Connection *connection,
//                         const char *url, const char *method,
//                         const char *version, const char *upload_data,
//                         size_t *upload_data_size, void **con_cls)
// {
//     // const char *page = "<html><body>Hello, browser!</body></html>";
//     const char *page = "hello world";
//     char resp_buffer[MAX_RESP_BUFF_SIZE];
//     struct MHD_Response *response;
//     int ret;
//     printf ("new %s request for %s using version %s\n", method, url, version);
//     MHD_get_connection_values (connection, MHD_HEADER_KIND, print_out_key,
//                                 NULL);
//     if ((0 != strcmp (method, "GET")) && (0 != strcmp (method, "POST"))) {
//         printf("unsupported method: %s\n", method);
//         return MHD_NO;
//     }
//     memset(resp_buffer, 0, sizeof (resp_buffer));
//     snprintf(resp_buffer, sizeof(resp_buffer), RESP_DATA_FORMAT, 0, page);
//     response = MHD_create_response_from_buffer (strlen (resp_buffer), (void *) resp_buffer,
//                     MHD_RESPMEM_PERSISTENT);
//     ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
//     MHD_destroy_response (response);
//     return ret;
// }

int 
main ()
{
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                    &answer_to_connection, NULL,
                    MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                    NULL, MHD_OPTION_END);
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
