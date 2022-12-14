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

   unsigned int Count;         /* The count number is an unsigned integer of length 2 bytes. This
        specifies a count of how many integers should be returned by the server */
   unsigned int requestID = 0; /*This is an unsigned integer of length 2 bytes. The first request packet sent by the client
has a Request ID of 1 and every successive request sent by the client increments the Request ID by 1 */

   struct Packet packetToSend;  /*THis is the packet that contains the Reuqets id and the count */

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

   int totalNumberPacketsReceived = 0; /* The total number of response packets received */
   int totalNumberBytesReceived = 0;   /*The total number of bytes received (as measured by the sum of the values returned by each
  recvfrom operation) */
   int sumSequenceNumber = 0;          /*This sum is obtainedby adding all Sequence Number fields as 16-bit
            unsigned integers, with any carry or overflow ignored. */
   uint32_t checksum = 0;              /* checksum of all the data integers received in all the response packets combined. This checksum
             is obtained by adding all the data integers as 32-bit unsigned integers, with any carry or overflow
             ignored*/
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
   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /*  server_port = atoi(argv[2]);
    printf("The port number for server is: %d \n", server_port); */
   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
          server_hp->h_length);
   server_addr.sin_port = htons(server_port);

   /*SendPackets is a variables that will change depending on the user. If user wants
   to continue requesting packets the value for sendPackets would be 1, but if the users
   does not want to request to the server the value will change to 0*/
   int sendPackets = 1;

   /*This while loop is in charged of closing the socket when the the user does not want
   to send more packets*/
   while (sendPackets == 1)
   {
      // Ask user how many integers would like to obtain from the server
      printf("Enter a count of integers would like to obtain from the server: ");
      scanf("%d", &Count);

      /*This if condition is in charged of making sure that the count variable given by the client
      does not exceed 2 bytes*/
      if (Count <= 0 || Count > 65535)
      {
         perror("Client: Input is not within the range of 0-65535, please re-enter the number again\n");
      }
      else
      {
         /*This if condition is in charged of making sure that the requestID variable
     does not exceed 2 bytes*/
         if (requestID < 65535)
         {
            requestID++;
            packetToSend.id = htons(requestID);
            packetToSend.Count = htons(Count);

            /*This method is in charged of sending the Structure p1, which containts the RequestID and the
     count number */
            bytes_sent = sendto(sock_client, &packetToSend, sizeof(packetToSend), 0,
                                (struct sockaddr *)&server_addr, sizeof(server_addr));

            printf("Waiting for response from server...\n");

            /*The division variable is in charged of doing the division between the count number giving by the user
            and 25. This is with the aim of determinig the lenght of the list of packets needed, becasue each
            payload will only can have 25 integers*/
            int division = (Count) / 25;
            int mod = Count % 25;
            int sizeList = 0;
            if (mod == 0)
            {
               sizeList = division;
            }
            else
            {
               sizeList = division + 1;
            }
            /*packets is the list of packets return fromt the server*/
            PacketServer packets[sizeList];
            /* PacketServer *packets = calloc(1, 108 * sizeList + 100); */

            /*Recvfrom is the methos whose objective is receving the information from the server*/
            bytes_recd = recvfrom(sock_client, &packets, sizeof(packets), 0,
                                  (struct sockaddr *)0, (int *)0);

            /*This ofr is in charged of matching the Request ID of the received packet with the
      Request ID of the request and should discard any responses for which the Request ID does not match*/
            for (int i = 0; i < sizeList; i++)
            {
               if (ntohs(packets[i].header.requestId) == requestID)
               {
                  totalNumberPacketsReceived += 1;
                  sumSequenceNumber += ntohs(packets[i].header.sequenceNumber);
                  for (int j = 0; j < 25; j++)
                  {
                     checksum += ntohs(packets[i].payload[j]);
                  }
               }
               else
               {
                  bytes_recd -= 108;
               }
            }

            /* This section will print the  information received */
            printf("-----------------------------------\n");
            printf("Request id: %d\n", requestID);
            printf("Sequence Number: %d\n", Count);
            printf("The total number of response packets received: %d\n", totalNumberPacketsReceived);
            printf("The total number of bytes  received: %d\n", bytes_recd);
            printf("The sum of all sequence Number: %d\n", sumSequenceNumber);
            printf("The checksum is %d\n", checksum);
            printf("-----------------------------------\n");
            /*This senctio will restart the counters to 0*/
            totalNumberPacketsReceived = 0;
            bytes_recd = 0;
            sumSequenceNumber = 0;
            checksum = 0;
         }
         else
         {
            perror("Client: The id is higher than 2 bytes\n");
            printf("Do you want to continue sending integers to the server? Type 1 for yes and 0 for No: ");
            scanf("%d", &sendPackets);
         }
      }
      printf("Do you want to continue sending integers to the server? Type 1 for yes and 0 for No: ");
      scanf("%d", &sendPackets);
   }

   close(sock_client);
}
