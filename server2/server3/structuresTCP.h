#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

/*This struct is in charged of storing the RequestId and the Count number giving in the request from the Client*/
struct Packet
{
    unsigned int stepNumber;
    unsigned int clientPortNumber;
    unsigned int serverPortNumber;
    unsigned int serverSecretCode;
    unsigned char text[80];
};

/* struct PacketClient
{
    unsigned int stepNumber;
    unsigned int serverPortNumber;
    unsigned int serverSecretCode;
    char location[1024];
}; */
/*THe Header struct is in charged of storing the RequestId, the sequenceNumber, the last, and the count */
/* struct Header {
    unsigned int stepNumber;
    unsigned int clientPortNumber;
    unsigned int  serverPortNumber;
    unsigned int serverSecretCode;
    char* text;
}; */
/*The packetServer struct is in charged of storing the header and the payload */
typedef struct packetServer
{
    /* struct Header header; */
    int payload[25];
} PacketServer;

int sumcheck = 0;
/*The CreatePAcketSenser is a method in charged of creating the packet that will be givin as an answer from the server
to the client.
@requestid = 'This is an unsigned integer of length 2 bytes. This
field must contain the same value that was sent by the client as the Request ID in the request packet'
@sequenceNumber =  'is also an unsigned integer of length 2 bytes. The first
response packet sent by the server has a Sequence Number of 1 and each successive response packet
increments the Sequence Number by 1.'
@last = is a unsigned integer of length 2 bytes. The value of this field is 0 if this is not the last packet in the
sequence of response packets returned by the server, and is 1 if this is the last response packet
@count = is unsigned integer of length 2 bytes, which specifies the number of data integers contained in the payload
*/
PacketServer *CreatePacketServer(unsigned int requestId, unsigned int sequenceNumber,
                                 unsigned int last, unsigned int count)
{
    PacketServer *packet = (PacketServer *)malloc(sizeof(PacketServer));

    /*   struct Header head;
      head.requestId=htons(requestId);
      head.sequenceNumber = htons(sequenceNumber);
      head.last =htons(last);
      head.count =htons(count);
      int position=0;
      while(position<count){
         int r = rand() % 4294967295;
         packet->payload[position]=htons(r);
         position++;
      }
      for (int i = position; i < 25; i++)
      {
           packet->payload[i]=htons(0);
      }

      packet->header = head;
   */
    return packet;
}

struct Packet createPacket(unsigned int stepNumber,
                           unsigned int clientPortNumber,
                           unsigned int serverPortNumber,
                           unsigned int serverSecretCode, char *text)
{
    struct Packet packet;
    packet.stepNumber = stepNumber;
    packet.clientPortNumber = clientPortNumber;
    packet.serverPortNumber = serverPortNumber;
    packet.serverSecretCode = serverSecretCode;
    strcpy(packet.text, text);
    return packet;
}

/* struct PacketClient createPacketFile(unsigned int stepNumber,
                                     unsigned int serverPortNumber,
                                     unsigned int serverSecretCode,
                                     char *location)
{
    struct PacketClient packet;
    packet.stepNumber = stepNumber;
    packet.serverPortNumber = serverPortNumber;
    packet.serverSecretCode = serverSecretCode;
    strcpy(packet.location, location);
    return packet;
} */

/*THe inserPAcket is the method in charged of iserting the packet to the list of packets that
will be sent to the client.

@requestid = 'This is an unsigned integer of length 2 bytes. This
field must contain the same value that was sent by the client as the Request ID in the request packet'
@sequenceNumber =  'is also an unsigned integer of length 2 bytes. The first
response packet sent by the server has a Sequence Number of 1 and each successive response packet
increments the Sequence Number by 1.'
@last = is a unsigned integer of length 2 bytes. The value of this field is 0 if this is not the last packet in the
sequence of response packets returned by the server, and is 1 if this is the last response packet
@count = is unsigned integer of length 2 bytes, which specifies the number of data integers contained in the payload
@ listpackets = is the list where the packet will be stored.
@position = it is the posistion in the list where the packet will be stored.

return int = is the next position where the next packet will be stored. THis position will increment
each time we add a new packet to the list.
*/
int insertPacket(unsigned int requestId, unsigned int sequenceNumber,
                 unsigned int last, unsigned int count, PacketServer *listpackets[], int position)
{
    PacketServer *packet = CreatePacketServer(requestId, sequenceNumber, last, count);
    listpackets[position] = packet;
    return position + 1;
}

void readFileClient(struct Packet *packetReceived, char *FileName)
{
    FILE *ptr;
    ptr = fopen("client.txt", "r");

    if (NULL == ptr)
    {

        printf("file can't be opened \n");
    }

    printf("content of this file are \n");
    char fileText[100];
    fgets(fileText, sizeof(fileText), ptr);

    int isINfo = 0;
    int isFound = 0;
    while (fgets(fileText, sizeof(fileText), ptr))
    {
        isINfo = 1;
        printf("%s", fileText);
        char *token = strtok(fileText, ",");

        unsigned int stepNumber;
        sscanf(token, "%u", &stepNumber);
        /*   printf(" %s\n", idSequence); */
        token = strtok(NULL, ",");
        unsigned int serverPort;
        sscanf(token, "%u", &serverPort);

        token = strtok(NULL, ",");
        unsigned int serverSecret;
        sscanf(token, "%u", &serverSecret);

        token = strtok(NULL, ",");
        printf("%s", token);

        struct Packet packet = createPacket(stepNumber, 0, serverPort,
                                            serverSecret, token);

        if (packetReceived->serverPortNumber == serverPort)
        {
            /*   stepNumber++; */
            /* char *message[80];
            strcpy(message, packetReceived->text);
            strcat(message, "\n"); */
            struct Packet packetFound = createPacket(packetReceived->stepNumber, packetReceived->serverPortNumber, packetReceived->serverPortNumber,
                                                     packetReceived->serverSecretCode, packetReceived->text);
            writeToFile(packetFound, FileName);
            isFound = 1;
        }
        else
        {
            writeToFile(packet, FileName);
        }
    }
    if (isINfo == 0 || isFound == 0)
    {   
         char *TravelLocation[80];
            strcpy(TravelLocation, packetReceived->text);
            strcat(TravelLocation, "\n");
        struct Packet FirstPacket = createPacket(packetReceived->stepNumber, packetReceived->clientPortNumber, packetReceived->serverPortNumber,
                                                 packetReceived->serverSecretCode, TravelLocation);
        writeToFile(FirstPacket, FileName);
    }

    system("mv tempClient.txt client.txt");
    system("touch tempClient.txt");
    /* char *filename = "tempClient.txt";

    // open the file for writing
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    // write to the text file

    fprintf(fp, "Step Number, Server Port, Server’s Secret Code, Server’s Travel Location\n");

    // close the file
    fclose(fp);*/

    fclose(ptr);
}

void writeToFile(struct Packet packet, char *FileName)
{
    if (strcmp(FileName, "client.txt") == 0)
    {
        char *filename = "tempClient.txt";

        // open the file for writing
        FILE *fp = fopen(filename, "a");
        int size = ftell (fp);
        if (size == 0)
        {
            fprintf(fp, "Step Number, Server Port, Server’s Secret Code, Server’s Travel Location\n");
        }
        // write to the text file

        fprintf(fp, "%d,%d,%d,%s",
                packet.stepNumber,
                packet.serverPortNumber,
                packet.serverSecretCode,
                packet.text);

        // close the file
        fclose(fp);
    }
    else
    {
        char *filename = "tempServer.txt";

        // open the file for writing
        FILE *file = fopen(filename, "a");
         int size = ftell (file);
        if (size == 0)
        {
            fprintf(file, "Step Number, Client Port, Client’s Visitor Name\n");
        }
        // write to the text file

        fprintf(file, "%d,%d,%s",
                packet.stepNumber,
                packet.clientPortNumber,
                packet.text);

        // close the file
        fclose(file);
    }

    return 0;
}

int findServerNumber(int serverPrtNumber, char *FileName)
{
    FILE *ptr;
    ptr = fopen(FileName, "r");

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }
    char fileText[100];
    fgets(fileText, sizeof(fileText), ptr);
    unsigned int stepNumber = 0;
    int isInside = 0;
    while (fgets(fileText, sizeof(fileText), ptr))
    {
        printf("%s", fileText);
        char *token = strtok(fileText, ",");
        sscanf(token, "%u", &stepNumber);
        /*   printf(" %s\n", idSequence); */
        token = strtok(NULL, ",");

        unsigned int SoC_Port;
        sscanf(token, "%u", &SoC_Port);
        if (serverPrtNumber == SoC_Port)
        {
            isInside = 1;
            return stepNumber;
        }
    }
    if (isInside == 0)
    {
        return 0;
    }
    else
    {
        return stepNumber;
    }
}

int readFileServer(struct Packet packetReceived, char *FileName)
{
    FILE *ptr;
    ptr = fopen("Server.txt", "r");

    if (NULL == ptr)
    {

        printf("file can't be opened \n");
    }

    char fileText[100];
    fgets(fileText, sizeof(fileText), ptr);

    int isINfo = 0;
    while (fgets(fileText, sizeof(fileText), ptr))
    {
        isINfo = 1;
        printf("%s", fileText);
        char *token = strtok(fileText, ",");

        unsigned int stepNumber;
        sscanf(token, "%u", &stepNumber);
        /*   printf(" %s\n", idSequence); */
        token = strtok(NULL, ",");
        unsigned int clientPort;
        sscanf(token, "%u", &clientPort);

        token = strtok(NULL, ",");
        printf("%s", token);

        struct Packet packet = createPacket(stepNumber, 0, clientPort,
                                            0, token);

        if (packetReceived.clientPortNumber == clientPort)
        {
            /* stepNumber++; */
            char *message[80];
            strcpy(message, packetReceived.text);
            strcat(message, "\n");
            struct Packet packetFound = createPacket(packetReceived.stepNumber, packetReceived.clientPortNumber, packetReceived.serverPortNumber,
                                                     packetReceived.serverSecretCode, message);
            writeToFile(packetFound, FileName);
        }
        else
        {
            writeToFile(packet, FileName);
        }
    }
    if (isINfo == 0)
    {
        struct Packet FirstPacket = createPacket(packetReceived.stepNumber, packetReceived.clientPortNumber, packetReceived.serverPortNumber,
                                                 packetReceived.serverSecretCode, packetReceived.text);
        writeToFile(FirstPacket, FileName);
    }
    system("mv tempServer.txt Server.txt");
    system("touch tempServer.txt");
    char *filename = "tempServer.txt";

    // open the file for writing
    /*  FILE *fp = fopen(filename, "w");
     if (fp == NULL)
     {
         printf("Error opening the file %s", filename);
         return -1;
     }
     // write to the text file

     fprintf(fp, "Step Number, Client Port, Client’s Visitor Name\n");

     // close the file
     fclose(fp); */

    fclose(ptr);
}