#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

/*This struct is in charged of storing the RequestId and the Count number giving in the request from the Client*/
struct Packet{
    unsigned int id;
    unsigned int Count;
};

/*THe Header struct is in charged of storing the RequestId, the sequenceNumber, the last, and the count */
struct Header {
    unsigned int requestId;
    unsigned int sequenceNumber;
    unsigned int  last;
    unsigned int count;
};
/*The packetServer struct is in charged of storing the header and the payload */
typedef struct packetServer{
    struct Header header;
    int payload[ 25 ];
} PacketServer;

int sumcheck=0;
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
PacketServer* CreatePacketServer(unsigned int requestId, unsigned int sequenceNumber,
    unsigned int  last, unsigned int count){
    PacketServer* packet= (PacketServer *) malloc(sizeof(PacketServer));
    
    struct Header head;
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

    return packet;   
}

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
    unsigned int  last, unsigned int count, PacketServer *listpackets[], int position){
    PacketServer* packet=CreatePacketServer(requestId, sequenceNumber, last, count);
    listpackets[position]=packet;
    return position+1;

}


