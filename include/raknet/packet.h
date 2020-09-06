#ifndef RZ_RAKNET_PACKET_H
#define RZ_RAKNET_PACKET_H

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "identifiers.h"

struct Packet {
    int size;
    char* buffer;
    struct sockaddr_in addr;
    int addrSize;
};

/*
 * packetNew
 *
 * @brief Creates a new packet and initializes the addrSize member
 */
struct Packet* packetNew();

/*
 * packetFree
 *
 * @brief Frees the packet including the buffer it contains
 */
void packetFree(struct Packet* packet);

/*
 * packetGetIdentifier
 *
 * @brief Returns the identifier of the packet (UnconnectedPing, UnconnectedPong, etc.)
 */
enum MessageID packetGetIdentifier(struct Packet* packet);

#endif // RZ_RAKNET_PACKET