#include "raknet/packet.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

struct RZPacket* rzPacketNew()
{
    struct RZPacket* packet = (struct RZPacket*)malloc(sizeof(struct RZPacket));
    if (!packet) {
        fprintf(stderr, "[ERROR] RZPacket malloc failed\n");
        return 0;
    }

    packet->addrSize = sizeof(struct sockaddr);

    return packet;
}

void rzPacketFree(struct RZPacket* packet)
{
    free(packet->buffer);
    free(packet);
}