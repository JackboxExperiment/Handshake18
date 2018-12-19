#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Creating WKP:\n");
  mkfifo("wkp", 0644);
  
  char s[HANDSHAKE_BUFFER_SIZE];
 
  printf("Connecting client...\n");
  int in_pipe = open("wkp", O_RDONLY);
  read(in_pipe, s, sizeof(s));
  printf("Enter Message: %s\n", s);
  remove("wkp");
  
  printf("Messaging client\n");
  int out_pipe = open(s, O_WRONLY);
  write(out_pipe, ACK, sizeof(ACK));
  
  printf("Receiving client message...\n");
  read(in_pipe, s, sizeof(s));
  printf("Message: %s\n", s);
  
  printf("Handshake shook\n");
  
  *to_client = out_pipe;
  return in_pipe;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Private piping:\n");
  mkfifo("private", 0644);

  printf("Connecting to server...\n");
  int out_pipe = open("wkp", O_WRONLY);
  write(out_pipe, "private", 8);

  printf("Receiving server message...\n");
  char message[HANDSHAKE_BUFFER_SIZE];
  int in_pipe = open("private", O_RDONLY);
  read(in_pipe, message, HANDSHAKE_BUFFER_SIZE);
  printf("Enter message: %s\n", message);
  remove("private");

  printf("Notifying server...\n");
  write(out_pipe, ACK, sizeof(ACK));

  printf("Handshake shook\n");

  *to_server = out_pipe;
  return in_pipe;
}
