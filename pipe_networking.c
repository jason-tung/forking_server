#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int wkp) {
  int mypid = getpid();
  remove("wkp");
  printf("SUBSERVER[%d]: removed wkp from main server.\n", mypid);
  char * pvt = malloc(BUFFER_SIZE);
  read(wkp, pvt, BUFFER_SIZE);
  printf("SUBSERVER[%d]: read pvt pipe name from wkp %s\n",mypid, pvt);
  int to_client;
  to_client = open(pvt, O_WRONLY);
  write(to_client, ACK, sizeof(ACK));
  printf("SUBSERVER[%d]: sent ack %s\n",mypid, ACK);
  char * hey = malloc(BUFFER_SIZE);
  read(wkp,hey,BUFFER_SIZE);
  printf("SUBSERVER[%d]: got message back %s\n",mypid, hey);
  printf("SUBSERVER[%d]: can send and receive\n",mypid);
  free(hey);
  return to_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  char * pvt_name = malloc(BUFFER_SIZE);
  sprintf(pvt_name, "%d", getpid());
  mkfifo(pvt_name, 0777);
  printf("CLIENT: mk pvt pipe finished. \n");
  *to_server = open("WELL", O_WRONLY);
    if(*to_server == -1){
        printf("CLIENT: can't open wkp. make sure server is running. %s\n", strerror(errno));
        remove(pvt_name);
        exit(1);
    }
  write(*to_server, pvt_name, BUFFER_SIZE);
  printf("CLIENT: sent pvt pipe to server\n");
  int pvt = open(pvt_name, O_RDONLY);
  char * servermsg = malloc(BUFFER_SIZE);
  read(pvt, servermsg, BUFFER_SIZE);
  printf("CLIENT: got ack from server %s\n", servermsg);
  remove(pvt_name);
  printf("CLIENT: removed pvt pipe\n");
  write(*to_server, ACK, BUFFER_SIZE);
  printf("CLIENT: sent servermsg back through wkp %s\n", servermsg);
  printf("CLIENT: can send and receive\n");
  return pvt;
}
