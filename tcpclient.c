/* tcp_ client.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2022 */

#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset, memcpy, and strlen */
#include <netdb.h>      /* for struct hostent and gethostbyname */
#include <sys/socket.h> /* for socket, connect, send, and recv */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include "structuresTCP.h"
#define STRING_SIZE 1024

#define CLIENT_TCP_PORT 31975
#define FILE_NAME "client.txt"
#define HOST_NAME "localhost"
#define VISITOR_NAME "MIGUEL-ROMERO"
#define SERV_TCP_PORT 65000
int main(void)
{

   int sock_client;                   /* Socket used by client */
   struct sockaddr_in client_addr;    /* Internet address structure that
                                          stores client address */
   unsigned short client_port;        /* Port number used by client (local port) */
   struct sockaddr_in server_addr;    /* Internet address structure that
                                          stores server address */
   struct hostent *server_hp;         /* Structure to store server's IP
                                          address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;        /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];         /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;               /* length of message */
   int bytes_sent, bytes_recd;         /* number of bytes sent or received */
   unsigned int stepNumber = 1;
   struct Packet packetToSend;

   unsigned int secretCode;  
   /*    char *hostName = "localhost";
      char *visitorName = "MIGUEL-ROMERO";
      unsigned int serverScretCode = 0;
      char travelLocation[7] = "Boston"; */

   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
   {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   client_port = CLIENT_TCP_PORT; /* This allows choice of any available local port */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   client_addr.sin_port = htons(client_port);

   /* bind the socket to the local client port */

   if (bind(sock_client, (struct sockaddr *)&client_addr,
            sizeof(client_addr)) < 0)
   {
      perror("Client: can't bind to local address\n");
      close(sock_client);
      exit(1);
   }

   /* printf("Enter hostname of server: ");
   scanf("%s", server_hostname); */
   if ((server_hp = gethostbyname(HOST_NAME)) == NULL)
   {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   /* printf("Enter port number for server: ");
   scanf("%hu", &server_port); */
   server_port = SERV_TCP_PORT;
   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
          server_hp->h_length);
   server_addr.sin_port = htons(server_port);

   /* connect to the server */

   if (connect(sock_client, (struct sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
   {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }

   // STEP 1

   /* user interface */

   /*  printf("Please input a sentence:\n");
    scanf("%s", sentence); */
   /*Create packet to send*/

   /* send message */

   int stepNum = findServerNumber(server_port, FILE_NAME);
   stepNum+=1;

   if (stepNum == 1 || stepNum==4)
   {  
      stepNum=1;
      packetToSend = createPacket(stepNum, CLIENT_TCP_PORT, 0, 0, "*");
      bytes_sent = send(sock_client, &packetToSend, sizeof(struct Packet), 0);
   }
   else if (stepNum == 2)
   {
      packetToSend = createPacket(stepNum, CLIENT_TCP_PORT, server_port, 0, "*");
      bytes_sent = send(sock_client, &packetToSend, sizeof(struct Packet), 0);
   }
   else{
   packetToSend = createPacket(stepNum, CLIENT_TCP_PORT, server_port, secretCode, VISITOR_NAME);
   bytes_sent = send(sock_client, &packetToSend, sizeof(struct Packet), 0);

   }
   /* get response from server */
   struct Packet *packetReceived = calloc(1, sizeof(struct Packet));
   bytes_recd = recv(sock_client, packetReceived, sizeof(struct Packet), 0);
   secretCode=packetReceived->serverSecretCode;
   readFileClient(packetReceived, FILE_NAME);

   /* struct PacketClient packetOne = createPacket(stepNumber, server_port,
                                                             0, TRAVEL_LOCATION); */
   printf("\nThe response from server is:\n");
   /* printf("The StepNUmber is %d\n, The clientPort is %d\n,  The Server Port is %d\n, the Server Secret Code is %d\n,  The messahe is %s\n", packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber, packetReceived->serverSecretCode,
          packetReceived->text); */
   /* readFile(struct Packet *packetReceived); */

   /* close the socket */

   close(sock_client);
}
