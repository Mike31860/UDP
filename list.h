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

typedef struct payload {
    unsigned int integer;
    struct payload* next;
} Payload;

typedef struct packetSender {
    struct Header header;
    struct ListPayload* payload;
    struct packetSender* next;
} PacketSender;

typedef struct listPacket{
    PacketSender* head;
    int length;
} ListPacket;

typedef struct listPayload{
    Payload* head;
    int length;
} ListPayload;

ListPayload* listpay;
ListPacket* listpacket;


Payload* CreatePayload(int integer){
    Payload* payload= (Payload *) malloc(sizeof(Payload));
    payload->integer = integer;
    payload->next = NULL;
    return payload;   
}

PacketSender* CreatePacketSender(unsigned int requestId, unsigned int sequenceNumber,
    unsigned int  last, unsigned int count){
    PacketSender* packet= (PacketSender *) malloc(sizeof(PacketSender));
    
    struct Header head;
    head.requestId=requestId;
    head.sequenceNumber = sequenceNumber;
    head.last =last;
    head.count =count;
    int number=1;
    while(number<=count){
       int r = rand() % 4294967295;
       insertIntegerPayload(r);
       number++;
    }
    packet->header = head;
    ListPayload* list= (ListPayload *) malloc(sizeof(ListPayload));
    list->head=listpay->head;
    list->length=listpay->length;
    packet->payload = list;
    listpay=(ListPayload*) malloc(sizeof(ListPayload));
    return packet;   
}

void insertIntegerPayload(int integer){
  
    Payload* payload= CreatePayload(integer);
    if(listpay==NULL){
        listpay= (ListPayload*) malloc(sizeof(ListPayload));
        listpay->length=0;
        listpay->head=NULL;
    }
    if(listpay->head==NULL){
        listpay->head=payload;
        listpay->length++;
    }
    else{
        Payload* point = listpay-> head;
        while(point-> next) {
            point= point->next;
        }
        point-> next=payload;
        listpay->length++;
        
    }

}

PacketSender*  insertPacket(unsigned int requestId, unsigned int sequenceNumber,
    unsigned int  last, unsigned int count){
    PacketSender* packet=CreatePacketSender(requestId, sequenceNumber, last, count);

    if(listpacket==NULL){
        listpacket= (ListPacket*) malloc(sizeof(ListPacket));
        listpacket->length=0;
        listpacket->head=NULL;
    }
    if(listpacket->head==NULL){
        listpacket->head=packet;
        listpacket->length++;
        return packet;
    }
    else{
        PacketSender* point = listpacket-> head;
        while(point-> next) {
            point= point->next;
        }
        point-> next=packet;
        listpacket->length++;
        return packet;
        
    }

}

void printListPackets(ListPacket* listpacket){

   PacketSender* point = listpacket->head;
   int size= listpacket->length;
   
        for(int i=0; i<size; i++){
        //    ListPayload* listpay=LastPacket->payload;
            struct Header header = point->header;
            printf("Header requestid: %d \n Header sequenceNumber: %d \n Header last: %d \n Header count: %d \n", header.requestId, header.sequenceNumber, header.last, header.count);
        }


}
