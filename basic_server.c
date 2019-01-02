#include "pipe_networking.h"

void sighand(int sig){
  if (sig == SIGINT){
    remove("wkp");
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
    signal(SIGINT,sighand);
    //printf("%s", secret_sauce("heyyy"));
  int to_client;
  int from_client;
  while(1) {
      from_client = server_handshake(&to_client);
      printf("SERVER: ready to take new client\n");
      char *friends = malloc(BUFFER_SIZE);
      while(read(from_client, friends, BUFFER_SIZE)){
          printf("DDDDDF%sFDDDDD",friends);
          printf("SERVER-message-receive: %s\n",friends);
          char * men = secret_sauce(friends);
          write(to_client, men, BUFFER_SIZE);
          printf("SERVER-message-sent: %s\n",men);
      }
  }
}
