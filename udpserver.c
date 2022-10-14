/* udp_server.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2022 */

#include <ctype.h>      /* for toupper */
#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset */
#include <sys/socket.h> /* for socket, sendto, and recvfrom */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include "list.h"
#define STRING_SIZE 1024

/* SERV_UDP_PORT is the port number on which the server listens for
   incoming messages from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_UDP_PORT 21975

int main(void)
{

   int sock_server; /* Socket on which server listens to clients */

   struct sockaddr_in server_addr; /* Internet address structure that
                                       stores server address */
   unsigned short server_port;     /* Port number used by server (local port) */

   struct sockaddr_in client_addr; /* Internet address structure that
                                       stores client address */
   unsigned int client_addr_len;   /* Length of client address structure */

   char sentence[STRING_SIZE];         /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;               /* length of message */
   int bytes_sent, bytes_recd;         /* number of bytes sent or received */
   unsigned int i;                     /* temporary loop variable */

   // Miguel
   struct Packet *packetMessage = calloc(1,sizeof(struct Packet));

  
   //
   /* open a socket */
   if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
      perror("Server: can't open datagram socket\n");
      exit(1);
   }
   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                      any host interface, if more than one
                                      are present */
   server_port = SERV_UDP_PORT;                     /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0)
   {
      perror("Server: can't bind to local address\n");
      close(sock_server);
      exit(1);
   }

   /* wait for incoming messages in an indefinite loop */

   printf("Waiting for incoming messages on port %hu\n\n",
          server_port);
   client_addr_len = sizeof(client_addr);
   for (;;)
   {

  //    printf("first");
      /* bytes_recd = recvfrom(sock_server, packetMessage, sizeof(*packetMessage), 0,
                            (struct sockaddr *)&client_addr, &client_addr_len);
      
      printf("Answer %d", bytes_recd);
      printf("Received Sentence is: %s\n     with length %d\n\n",
                         sentence, bytes_recd);
       printf("Received packet is id: %d and the countr is %d\n",  packetMessage->id, packetMessage->Count);                   
     
 */
      bytes_recd = recvfrom(sock_server, packetMessage, sizeof(*packetMessage), 0,
                            (struct sockaddr *)&client_addr, &client_addr_len);
      printf("Received packet is id: %d and the countr is %d\n",  packetMessage->id, packetMessage->Count);
      
     /*  int id=packetMessage->id;
      int count=packetMessage->Count;
      int division=(count)/25;
      int mod=count%25;
      printf("Division %d", division);
      int sequenceNumber=1;
     
      for(int i=0; i<division; i++)
      {
          if(mod==0){
          PacketSender* packet=insertPacket(id,  sequenceNumber, 1, 25);
          ListPayload* listpay=packet->payload;
          sequenceNumber++;

          }else{
            PacketSender* packet=insertPacket(id,  sequenceNumber, 0, 25);
            ListPayload* listpay=packet->payload;
            sequenceNumber++;
          }
         
      }
      PacketSender* LastPacket=insertPacket(id,  sequenceNumber, 1, mod );
      ListPayload* listpay=LastPacket->payload;


      printf("first element %d", listpay->head->integer); */

      /* prepare the message to send */
     /*  printf("Received packet is id: %d\n with count %d\n\n",
             packetMessage->id, packetMessage->Count); */
      /*   msg_len = bytes_recd;
        for (i=0; i<msg_len; i++)
           modifiedSentence[i] = toupper (sentence[i]); */

      /* send message */
 bytes_sent = sendto(sock_server, (struct Packet*)&packetMessage, (sizeof(packetMessage)), 0,
                  (struct sockaddr*) &client_addr, client_addr_len);

     /*     bytes_sent = sendto(sock_server, (struct ListPacket*)&listpacket, (sizeof(listpacket)), 0,
                  (struct sockaddr*) &client_addr, client_addr_len); */
   }
}
