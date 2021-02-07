//Main Entry Point for Daemon

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/serve.h"
#include "../include/fetch.h"

static int done;
void  INThandler(int sig)
{
  done = 1;
}

int main(int argc, char ** argv){
  char *key_pem, *cert_pem;
  struct MHD_Daemon* httpd;
  sigset_t mask;
  sigemptyset (&mask); //Allow all signals

  done = 0;
  signal(SIGINT, INThandler);
  key_pem = load_file("key.pem");
  cert_pem = load_file("cert.pem");
  httpd = http_start(8080, key_pem, cert_pem); //Start http server
  while (!done){
    sigsuspend (&mask); //Suspend this thread till signal recieved
  }
  MHD_stop_daemon(httpd);
  free (key_pem);
  free (cert_pem);
  printf("Done. Exiting.\n\r");
  exit(0);
}
