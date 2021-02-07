//Source: https://www.gnu.org/software/libmicrohttpd/tutorial.html#tlsauthentication_002ec
#include "../include/serve.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define PAGE "<html><head><title>libmicrohttpd demo</title>"	\
  "</head><body>libmicrohttpd demo</body></html>"

int print_out_key (void *cls, enum MHD_ValueKind kind, 
                   const char *key, const char *value)
{
  printf ("%s: %s\n", key, value);
  return MHD_YES;
}


int ahc_echo(void * cls,
	     struct MHD_Connection * connection,
	     const char * url,
	     const char * method,
	     const char * version,
	     const char * upload_data,
	     size_t * upload_data_size,
	     void ** ptr) {
  static int dummy;
  const char * page = PAGE;
  struct MHD_Response * response;
  int ret;
  if (0 != strcmp(method, "GET"))
    return MHD_NO; /* unexpected method */
  if (&dummy != *ptr)
    {
      /* The first time only the headers are valid,
         do not respond in the first round... */
      *ptr = &dummy;
      return MHD_YES;
    }
  if (0 != *upload_data_size){
    return MHD_NO; /* upload data in a GET!? */
  }
  printf("%s\n",url);
  MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);
  *ptr = NULL; /* clear context pointer */
  response = MHD_create_response_from_buffer (strlen(page),
                                              (void*) page,
  					      MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response(connection,
			   MHD_HTTP_OK,
			   response);
  MHD_destroy_response(response);
  return ret;
}

struct MHD_Daemon* http_start(int port, char *key_pem, char *cert_pem) {
  struct MHD_Daemon * d = NULL;
  if ((key_pem == NULL) || (cert_pem == NULL)){ //If no cert is provided, do not use ssl
    d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
			 port,
			 NULL,
			 NULL,
			 &ahc_echo,
			 NULL,
			 MHD_OPTION_END);
  } else { //If a cert is provided, use ssl
    d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION | MHD_USE_SSL,
			 port,
			 NULL,
			 NULL,
			 &ahc_echo,
			 NULL,
			 MHD_OPTION_HTTPS_MEM_KEY, key_pem,
			 MHD_OPTION_HTTPS_MEM_CERT, cert_pem,
			 MHD_OPTION_END);
  }
  if (d == NULL){
    printf("Server failed to start\n");
  }
  else {
    printf("Server started on port: %d\n",port);
  }
    
  return d;
}

char* load_file( char* filename ){
  long get_file_size (const char *filename)
  {
    FILE *fp;

    fp = fopen (filename, "rb");
    if (fp)
      {
	long size;

	if ((0 != fseek (fp, 0, SEEK_END)) || (-1 == (size = ftell (fp))))
	  size = 0;

	fclose (fp);

	return size;
      }
    else
      return 0;
  }

  FILE *fp;
  char *buffer;
  long size;

  size = get_file_size (filename);
  if (0 == size) return NULL;

  fp = fopen (filename, "rb");
  if (! fp) return NULL;

  buffer = malloc (size + 1);
  if (! buffer)
    {
      fclose (fp);
      return NULL;
    }
  buffer[size] = '\0';

  if (size != (long) fread (buffer, 1, size, fp))
    {
      free (buffer);
      buffer = NULL;
    }

  fclose (fp);
  return buffer;

}
