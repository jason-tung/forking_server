#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  char * send_buff = malloc(BUFFER_SIZE);
  char * rec_buff = malloc(BUFFER_SIZE);
  while(1){
    printf("input: ");
    //scanf(send_buff, stdin);
    fgets(send_buff, BUFFER_SIZE, stdin);
    send_buff[strlen(send_buff) - 1] = '\0';
    write(to_server, send_buff, BUFFER_SIZE);
    printf("CLIENT-message-send: %s\n", send_buff);
    read(from_server, rec_buff, BUFFER_SIZE);
    printf("CLIENT-message-received: %s\n", rec_buff);
  }
}
