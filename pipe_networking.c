#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("wkp", 0777);
  printf("SERVER: mk wkp finished. waiting for client.\n");
  int wkp = open("wkp", O_RDONLY);
  char * pvt = malloc(BUFFER_SIZE);
  read(wkp, pvt, BUFFER_SIZE);
  printf("SERVER: read pvt pipe name from wkp %s\n", pvt);
  remove("wkp");
  printf("SERVER: removed wkp\n");
  *to_client = open(pvt, O_WRONLY);
  write(*to_client, ACK, sizeof(ACK));
  printf("SERVER: sent ack %s\n", ACK);
  char * hey = malloc(BUFFER_SIZE);
  read(wkp,hey,BUFFER_SIZE);
  printf("SERVER: got message back %s\n", hey);
  printf("SERVER: can send and receive\n");
  free(hey);
  return wkp;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("pvt", 0777);
  printf("CLIENT: mk pvt pipe finished. \n");
  *to_server = open("wkp", O_WRONLY);
    if(*to_server == -1){
        printf("CLIENT: can't open wkp. make sure server is running. %s\n", strerror(errno));
        remove("pvt");
        exit(1);
    }
  write(*to_server, "pvt", BUFFER_SIZE);
  printf("CLIENT: sent pvt pipe to server\n");
  int pvt = open("pvt", O_RDONLY);
  char * servermsg = malloc(BUFFER_SIZE);
  read(pvt, servermsg, BUFFER_SIZE);
  printf("CLIENT: got ack from server %s\n", servermsg);
  remove("pvt");
  printf("CLIENT: removed pvt pipe\n");
  write(*to_server, ACK, BUFFER_SIZE);
  printf("CLIENT: sent servermsg back through wkp %s\n", servermsg);
  printf("CLIENT: can send and receive\n");
  return pvt;
}
