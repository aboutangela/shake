#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("making wkp...");
  if(mkfifo("mario", 0666))
    printf("success!\n");
  else
    printf("failed\n");

  int from = open("mario", O_RDONLY);
  printf("client connected\n");
  char priv[HANDSHAKE_BUFFER_SIZE];
  read(from,priv,sizeof(priv));
  printf("received message: %s\n", priv);
  remove("mario");

  int to = open(priv, O_WRONLY);
  write(to,ACK,sizeof(ACK));

  char message[HANDSHAKE_BUFFER_SIZE];
  read(from,message,sizeof(message));
  printf("received message: %s\n", message);

  *to_client = to;
  return from;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  //int priv = getpid();
  printf("making private fifo...");
  char priv[HANDSHAKE_BUFFER_SIZE];
  sprintf(priv,"%d",getpid());
  if(mkfifo(priv, 0666))
    printf("success!\n");
  else
    printf("failed\n");

  int to = open("mario", O_WRONLY);
  printf("connected to wkp\n");
  write(to,priv,sizeof(priv));

  int from = open(priv, O_RDONLY);
  char message[HANDSHAKE_BUFFER_SIZE];
  read(from,message,sizeof(message));

  printf("received message: %s\n", message);
  remove(priv);

  write(to,ACK,sizeof(ACK));

  *to_server = to;
  return from;
}
