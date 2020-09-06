#ifndef RAZOR_RAKNET_PACKET_HANDLER_H
#define RAZOR_RAKNET_PACKET_HANDLER_H

#include "packet.h"
#include "listener.h"

void handlePacket(struct Listener* listener, struct Packet* pk);

#endif // RAZOR_RAKNET_PACKET_HANDLER_H
