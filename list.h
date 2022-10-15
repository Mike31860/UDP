#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

struct Packet{
    unsigned int id;
    unsigned int Count;
};

struct Header {
    unsigned int requestId;
    unsigned int sequenceNumber;
    unsigned int  last;
    unsigned int count;
};

typedef struct packetSender {
    struct Header header;
    int payload[ 25 ];
} PacketSender;

typedef struct listPacket{
    PacketSender* head;
    int length;
} ListPacket;


ListPacket* listpacket;



PacketSender* CreatePacketSender(unsigned int requestId, unsigned int sequenceNumber,
    unsigned int  last, unsigned int count){
    PacketSender* packet= (PacketSender *) malloc(sizeof(PacketSender));
    
    struct Header head;
    head.requestId=requestId;
    head.sequenceNumber = sequenceNumber;
    head.last =last;
    head.count =count;
    int number=0;
    while(number<count){
       int r = rand() % 4294967295;
       packet->payload[number]=r;
       number++;
    }
    packet->header = head;

    return packet;   
}


int insertPacket(unsigned int requestId, unsigned int sequenceNumber,
    unsigned int  last, unsigned int count, PacketSender *listpacketssend[], int position){
    PacketSender* packet=CreatePacketSender(requestId, sequenceNumber, last, count);
    listpacketssend[position]=packet;
    return position+1;

}


