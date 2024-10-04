#include <libnetconf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <libnetconf_ssh.h>


#define ARGUMENTS "s:g:h"
char some_msg[4096];
#define INSTRUCTION(output,format,args...) {snprintf(some_msg,4095,format,##args);fprintf(output,"\n  %s",some_msg);}
#define ERROR(function,format,args...) {snprintf(some_msg,4095,format,##args);fprintf(stderr,"%s: %s\n",function,some_msg);}


char * pw(const char *u, const char *h) { 
 char *pass = "Dfkmr09";
 char *p = (char *)malloc(strlen(pass));
 strcpy(p, pass);
 return p; 
}

void usage(char* progname) {
    printf("Get netconf configuration and state data from the netconf server.\n");
    printf("Usage: %s [-h] [hostname] -s set_of_cmd | -g filter\n", progname);
    printf("-h             Show this help\n");
    printf("-s set_of_cmd  configure device\n");
    printf("-g filter      get config by filter\n");
}

int main(int argc, char **argv) {

struct nc_session *ncsession;
nc_rpc *ncrpc = NULL;
nc_reply *ncreply = NULL;
nc_msgid id;
NC_MSG_TYPE msg_type;
const char* operation;
int ret = EXIT_SUCCESS;
const char* output_file;
FILE* output;
char *data = NULL;
FILE* out_stream;
NC_DATASTORE target, source = NC_DATASTORE_ERROR;
char *host = argv[1];
char *cmd = argv[2];
char *cmdg;
int fs = 0;
int fg = 0;
int c;

  if(argc < 3) {
    printf("Too few arguments\n");
    usage(argv[0]);
    exit(1);
  }

  while ((c = getopt(argc, argv, ARGUMENTS)) != -1) {
    switch (c) {
      case 'g':
         cmdg = optarg;
         fg = 1;
         break;
      case 's':
         cmd = optarg;
         fs = 1;
         break;
      case 'h':
         usage(argv[0]);
         return EXIT_SUCCESS;
      default:
         fprintf(stderr, "unknown argument -%c", optopt);
         break;
    }
  }

  if(fs && fg) {
    return EXIT_SUCCESS;
   }

   const char *rr0 = "<rpc> \
           <load-configuration action=\"set\"> \
           <configuration-set>";
           //set system host-name PE0
   const char *rr2 = "</configuration-set> \
           </load-configuration> \
           </rpc>";

/*
           </rpc>";
*/
 if(fs) {
   char *rr = (char *)malloc(strlen(rr0) + 1 + strlen(cmd) + 1 + strlen(rr2) + 1);
   bzero(rr, strlen(rr0) + 1 + strlen(cmd) + 1 + strlen(rr2) + 1);
   strcpy(rr, rr0);
   strcat(rr, cmd);
   strcat(rr, rr2);

   printf("host = %s\n", host);
   printf("rr = %s\n", rr);

   nc_verbosity(NC_VERB_DEBUG);

  char *(*func)(const char *username, const char *hostname) = pw;

  nc_ssh_pref(NC_SSH_AUTH_PASSWORD, 10);
  nc_ssh_pref(NC_SSH_AUTH_INTERACTIVE, -1);
  nc_ssh_pref(NC_SSH_AUTH_PUBLIC_KEYS, -1);
  nc_callback_sshauth_password(func);   
  ncsession = nc_session_connect(host, 0, "noc", 0); 

  if(ncsession == NULL) {
    printf("ncsession is NULL\n");
    exit(1);
  }

  if (nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
    printf("NC_SESSION_STATUS_WORKING\n");
  }

  //ncrpc nc_rpc_lock(NC_DATASTORE_CONFIG,);   
  //id = nc_session_send_rpc(ncsession, ncrpc);

  //ncrpc = nc_rpc_generic(rr);
  ncrpc = nc_rpc_build(rr, ncsession);
  id = nc_session_send_rpc(ncsession, ncrpc);

  printf("rpc is: %s\n", nc_rpc_dump(ncrpc));

  ncrpc = nc_rpc_commit();
  //id = nc_session_send_rpc(ncsession, ncrpc);
  msg_type = nc_session_send_recv(ncsession, ncrpc, &ncreply);


  printf("rpc is: %s\n", nc_rpc_dump(ncrpc));

  if (nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
    printf("NC_SESSION_STATUS_WORKING\n");
  }

  free(rr);
  nc_rpc_free(ncrpc);
  nc_session_free(ncsession);   
 } else if(fg) {
   printf("cmdg: %s\n", cmdg); 

  char *gcrr = (char *)malloc(strlen(cmdg) + 1);
  bzero(gcrr, strlen(cmdg) + 1);
  strcpy(gcrr, cmdg);

  printf("host = %s\n", host);
  printf("gcrr = %s\n", gcrr);

  nc_verbosity(NC_VERB_DEBUG);

  char *(*func)(const char *username, const char *hostname) = pw;

  nc_ssh_pref(NC_SSH_AUTH_PASSWORD, 10);
  nc_ssh_pref(NC_SSH_AUTH_INTERACTIVE, -1);
  nc_ssh_pref(NC_SSH_AUTH_PUBLIC_KEYS, -1);
  nc_callback_sshauth_password(func);   
  ncsession = nc_session_connect(host, 0, "noc", 0); 

 if(ncsession == NULL) {
  printf("ncsession is NULL\n");
  exit(1);
 }

 if (nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
  printf("NC_SESSION_STATUS_WORKING\n");
 }

 ncrpc = nc_rpc_build(gcrr, ncsession);
 id = nc_session_send_rpc(ncsession, ncrpc);

 printf("rpc is: %s\n", nc_rpc_dump(ncrpc));

 msg_type = nc_session_send_recv(ncsession, ncrpc, &ncreply);


 printf("rpc is: %s\n", nc_rpc_dump(ncrpc));
 fprintf(stdout, "%s\n", data = nc_reply_get_data(ncreply));

 if (nc_session_get_status(ncsession) == NC_SESSION_STATUS_WORKING) {
  printf("NC_SESSION_STATUS_WORKING\n");
 }

 free(gcrr);
 nc_rpc_free(ncrpc);
 nc_session_free(ncsession);   
 }
}
