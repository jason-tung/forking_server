#include "pipe_networking.h"

static void sighandler(int sig){
  if (sig == SIGINT){
    remove("WELL");
    exit(0);
  }
}

char * secret_sauce(char * ingredients){
    char * ret = malloc(BUFFER_SIZE);
    strcpy(ret, ingredients);
    strcat(ret, "sauce");
    return ret;
}

int main() {
    signal(SIGINT,sighandler);
    //printf("%s", secret_sauce("heyyy"));
  int to_client;
  int from_client;
  while(1) {

      printf("SERVER: making new WKP\n");
      mkfifo("WELL",0777);
      printf("SERVER: new wkp done.\n");
      printf("SERVER: waiting for client.\n");
      int wkp = open("WELL", O_RDONLY);
      printf("SERVER: found new client\n");
      if (fork()){
          printf("SERVER: deleting old wkp\n");
          close(wkp);
          remove("WELL");

      }
      else {
          int mypid = getpid();
          printf("SUBSERVER[%d]: READY TO SERVE\n",mypid);
          to_client = server_handshake(wkp);
          char *friends = malloc(BUFFER_SIZE);
          while (read(wkp, friends, BUFFER_SIZE)) {
              //printf("DDDDDF%sFDDDDD",friends);
              printf("SUBSERVER[%d]-message-receive: %s\n",mypid, friends);
              char *men = secret_sauce(friends);
              write(to_client, men, BUFFER_SIZE);
              printf("SUBSERVER[%d]-message-sent: %s\n",mypid, men);
          }
      }
  }
}
