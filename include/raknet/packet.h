#ifndef RZ_PACKET_H
#define RZ_PACKET_H

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

struct RZPacket {
    int size;
    char* buffer;
    struct sockaddr_in addr;
    int addrSize;
};

struct RZPacket* rzPacketNew();
void rzPacketFree(struct RZPacket* packet);

#endif // RZ_PACKET