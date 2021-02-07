#include <microhttpd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#ifndef serv_H
#define serv_H


struct MHD_Daemon* http_start(int port, char *key_pem, char *cert_pem);
char* load_file(char* filename);



#endif
