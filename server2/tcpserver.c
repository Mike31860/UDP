/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2022 */

#include <ctype.h>      /* for toupper */
#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset */
#include <sys/socket.h> /* for socket, bind, listen, accept */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include "structuresTCP.h"
#define STRING_SIZE 1024
#define TRAVEL_LOCATION "Boston"
#define SERVER_SECRET_CODE 75
#define FILE_NAME "Server.txt"
/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 64000
/* #define SERV_TCP_PORT 31975 */

int main(void)
{

   int sock_server;     /* Socket on which server listens to clients */
   int sock_connection; /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr; /* Internet address structure that
                                       stores server address */
   unsigned int server_addr_len;   /* Length of server address structure */
   unsigned short server_port;     /* Port number used by server (local port) */

   struct sockaddr_in client_addr; /* Internet address structure that
                                       stores client address */
   unsigned int client_addr_len;   /* Length of client address structure */

   char sentence[STRING_SIZE];         /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;               /* length of message */
   int bytes_sent, bytes_recd;         /* number of bytes sent or received */
   unsigned int i;                     /* temporary loop variable */

   /* open a socket */

   struct Packet *packetReceived = calloc(1, sizeof(struct Packet));

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
   {
      perror("Server: can't open stream socket");
      exit(1);
   }

   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                      any host interface, if more than one
                                      are present */
   server_port = SERV_TCP_PORT;                     /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0)
   {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0)
   {                                     /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);

   client_addr_len = sizeof(client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;)
   {

      sock_connection = accept(sock_server, (struct sockaddr *)&client_addr,
                               &client_addr_len);
      /* The accept function blocks the server until a
         connection request comes from a client */
      if (sock_connection < 0)
      {
         perror("Server: accept() error\n");
         close(sock_server);
         exit(1);
      }
      /*Step 1*/
      /* receive the message */

      bytes_recd = recv(sock_connection, packetReceived, sizeof(struct Packet), 0);
      /*  bytes_sent = send(sock_connection, &packetReceived, sizeof(packetReceived), 0); */

      printf("The StepNUmber is %d\n, The clientPort is %d\n,  The Server Port is %d\n, the Server Secret Code is %d\n,  The messahe is %s\n", packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber, packetReceived->serverSecretCode,
             packetReceived->text);

      int stepNum = findServerNumber(server_port, FILE_NAME);

      struct Packet packet;
      if (packetReceived->stepNumber == 1)
      {
         packet = createPacket(packetReceived->stepNumber, packetReceived->clientPortNumber, SERV_TCP_PORT, 0, "*");
         bytes_sent = send(sock_connection, &packet, sizeof(struct Packet), 0);
         readFileServer(packet, FILE_NAME);
      }
      else if (packetReceived->stepNumber == 2)
      {
         packet = createPacket(packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber, SERVER_SECRET_CODE, "*");
         bytes_sent = send(sock_connection, &packet, sizeof(struct Packet), 0);
         readFileServer(packet, FILE_NAME);
      }
      else
      {

         packet = createPacket(packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber, SERVER_SECRET_CODE, TRAVEL_LOCATION);
         bytes_sent = send(sock_connection, &packet, sizeof(struct Packet), 0);
         struct Packet packetToWrite;
         packetToWrite = createPacket(packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber, SERVER_SECRET_CODE, packetReceived->text);
         readFileServer(packetToWrite, FILE_NAME);
      }

      close(sock_connection);

      /* close the socket */
   }
}
