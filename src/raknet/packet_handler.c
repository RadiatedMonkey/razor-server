#include "raknet/packet_handler.h"

#include <stdio.h>

#include "raknet/identifiers.h"

void handlePacket(struct Listener* listener, struct Packet* pk)
{
    (void)listener;
    enum MessageID id = packetGetIdentifier(pk);

    printf("Packet id: %i\n", id);
}