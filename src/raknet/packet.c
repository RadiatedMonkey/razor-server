#include "raknet/packet.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "raknet/identifiers.h"

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

struct Packet* packetNew()
{
    struct Packet* packet = (struct Packet*)malloc(sizeof(struct Packet));
    if (!packet) {
        fprintf(stderr, "[ERROR] Packet malloc failed\n");
        return 0;
    }

    packet->addrSize = sizeof(struct sockaddr);

    return packet;
}

void packetFree(struct Packet* packet)
{
    free(packet->buffer);
    free(packet);
}

enum MessageID packetGetIdentifier(struct Packet* pk)
{
    (void)pk;
//    if (pk->buffer == 0)
//        return 0;
//
//    if ((unsigned char)pk->buffer[0] == ID_TIMESTAMP) {
//        assert(pk->size > sizeof(MessageID) + sizeof(RZTimestamp));
//        return (unsigned char) pk->buffer[sizeof(RZMessageID) + sizeof(RZTimestamp)];
//    } else
//        return (unsigned char)pk->buffer[0];

    return 0;
}