/* udp_client.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2022 */

#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset, memcpy, and strlen */
#include <netdb.h>      /* for struct hostent and gethostbyname */
#include <sys/socket.h> /* for socket, sendto, and recvfrom */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include "list.h"
#include <errno.h>
#define STRING_SIZE 1024

int main(void)
{

   int sock_client; /* Socket used by client */

   unsigned int Count;
   unsigned int requestID = 0;
   int numberRange = 0;

   struct Packet p1;

   struct sockaddr_in client_addr; /* Internet address structure that
                                       stores client address */
   unsigned short client_port;     /* Port number used by client (local port) */

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

   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
   {
      perror("Client: can't open datagram socket\n");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port.
            The local address initialization and binding is done automatically
            when the sendto function is called later, if the socket has not
            already been bound.
            The code below illustrates how to initialize and bind to a
            specific local port, if that is desired. */

   /* initialize client address information */

   client_port = 0; /* This allows choice of any available local port */

   /* Uncomment the lines below if you want to specify a particular
             local port: */
   /*
   printf("Enter port number for client: ");
   scanf("%hu", &client_port);
   */

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

   /* end of local address initialization and binding */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL)
   {
      perror("Client: invalid server hostname\n");
      close(sock_client);
      exit(1);
   }

   while (numberRange == 0)
   {
      // Ask user how many integers would like to obtain from the server
      printf("Enter a count of integers would like to obtain from the server: ");
      scanf("%d", &Count);
      if (Count <= 0 || Count > 65535)
      {
         perror("Client: Input is not within the range of 0-65535, please re-enter the number again\n");
      }
      else
      {
         if (requestID < 65535)
         {
            numberRange = 1;
            requestID++;
            p1.id = requestID;
            p1.Count = Count;
            printf("id is %d and Count is %d \n", p1.id, p1.Count);
         }
         else
         {
            perror("Client: The id is higher than 65535\n");
         }
      }
   }

  /*  printf("Enter port number for server: ");
   scanf("%hu", &server_port); */

   server_port=21975;
   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
          server_hp->h_length);
   server_addr.sin_port = htons(server_port);

   /* user interface */

 /*   printf("Please input a sentence:\n");
   scanf("%s", sentence);
   msg_len = strlen(sentence) + 1; */

   /* send message */

   /* bytes_sent = sendto(sock_client, sentence, msg_len, 0,
                       (struct sockaddr *)&server_addr, sizeof(server_addr)); */

   bytes_sent = sendto(sock_client, (struct Packet*)&p1, (sizeof(p1)), 0,
                       (struct sockaddr *)&server_addr, sizeof(server_addr));                    

   /* get response from server */

   printf("Waiting for response from server...\n");
  struct  Packet* packetuu= calloc(1,sizeof(struct Packet));
   bytes_recd = recvfrom(sock_client, packetuu, sizeof(*packetuu), 0,  
   (struct sockaddr *)0, (int*)0);
 /*   if(bytes_recd<0)
   {
      printf("\nUDP Broadcast not received ERROR CODE : %s\n",strerror(errno));

    
   } */
 /*   printf("Received packet is id: %d and the countr is %d\n",  packetuu->id, packetuu->Count); */
     
   ListPacket *listpacket2 = (ListPayload *)  malloc(sizeof(ListPacket));
   bytes_recd = recvfrom(sock_client, listpacket2, sizeof(*listpacket2), 0,
                         (struct sockaddr *)0, (int*)0);
 /*   printf("\nThe response from server is: %d\n", listpacket2->length); */
 /*   PacketSender* point = listpacket2->head;
   int size= listpacket2->length;
   
        for(int i=0; i<size; i++){
        //    ListPayload* listpay=LastPacket->payload;
            struct Header header = point->header;
            printf("Header requestid: %d \n Header sequenceNumber: %d \n Header last: %d \n Header count: %d \n", header.requestId, header.sequenceNumber, header.last, header.count);
        } */



  // printListPackets(&listpacket);
   //printf("%s\n\n", modifiedSentence);

   /* close the socket */

   close(sock_client);
}
