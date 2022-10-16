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


   struct Packet *packetMessage = calloc(1, sizeof(struct Packet)); /* packetMessage is a struct sent from the client to the server*/

   
   int totalNumberPacketsTransmitted=0; /* The total number of response packets transmitted*/

   int totalNUmberBytesTransmitted=0; /*The total number of bytes transmitted  */ 

   int sumSequenceNumber=0; /* The sum of all the Sequence Number fields of the transmitted response packets. */ 

   uint32_t checksum=0; /* A checksum of all the data integers transmitted in all the response packets combined. */ 

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

      /* The recvfrom is the method in charged of receiving the struct packet from the client  */ 

      bytes_recd = recvfrom(sock_server, packetMessage, sizeof(packetMessage), 0,
                            (struct sockaddr *)&client_addr, &client_addr_len);

      /*The packet has been received*/
      printf("Received packet is id: %d and the count is %d\n", packetMessage->id, packetMessage->Count);

      int id = packetMessage->id; /*RequestId sent from the Client*/
      int count = packetMessage->Count; /*The count number sent from the client*/
      int division = (count) / 25; /* The division variable is to know how many packets will be needed to send 
      to the client, because each payload can not exceed 25 integers.  */
      int mod = count % 25; /* The module variable is to obtain the remainder of the division, which 
      will result in another packet to be sent to the client*/

      int sequenceNumber = 1; /* The first response packet sent by the server has a Sequence Number of 1 and each successive response packet increments the Sequence Number by 1. */
      int sizeList=0; /* The sizeList is the size of the list, it means how many packets will be sent to the client */

      if(mod==0){
         sizeList=division;
      }else{
         sizeList=division+1;
      } 

      PacketServer *listPackets[sizeList]; /* This is the list of packets to be sent to the client*/
      int position=0;

      /*This for is in charged of inserting the packets into the list. Also, it increments the sequence
      number and change the last value depending on the last packet */
      for (int i = 0; i < division; i++)
      {
         if (i == (division - 1)&& mod==0)
         {
            position = insertPacket(id, sequenceNumber, 1, 25, listPackets,position);
            sequenceNumber++;
         }
         else
         {
            position = insertPacket(id, sequenceNumber, 0, 25, listPackets,position);
            sequenceNumber++;
         }
      }
      if (mod != 0)
      {
         position= insertPacket(id, sequenceNumber, 1, mod, listPackets,position);

      }

   
     
      PacketServer packets[sizeList]; /* This is another list, but this is a struct that does not have pointers. 
      THis is the struct that we will send to the client*/ 
      for (int i = 0; i < sizeList; i++)
      {
         packets[i] = *listPackets[i];
         for (size_t j = 0; j < 25; j++)
         {
            checksum+=  packets[i].payload[j];
         }
                  
      }
      /*THis method is in charged of sending the struct to the client. The size would be 8 bytes for the header
      and 100 bytes for the payload (4*25 Integers), with a total of 108 bytes each packet*/
      bytes_sent = sendto(sock_server, &packets, 108 * sizeList, 0,
                          (struct sockaddr *)&client_addr, client_addr_len);
      
      totalNumberPacketsTransmitted+=sizeList;
      totalNUmberBytesTransmitted+=bytes_sent;
      
   }
}
