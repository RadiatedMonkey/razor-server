#include "listener.h"

#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>

#include "raknet/packet.h"

struct RZListener* rzListenerNew(unsigned short port, unsigned short maxConnections)
{
    struct RZListener* listener = (struct RZListener*)malloc(sizeof(struct RZListener));
    if (!listener) {
        fprintf(stderr, "[ERROR] RZListener malloc failed\n");
        return 0;
    }

    listener->clients = (RZClientSocket*)malloc(sizeof(RZClientSocket) * maxConnections);
    if (!listener->clients) {
        fprintf(stderr, "[ERROR] Clients malloc failed\n");
        free(listener);
        return 0;
    }

    listener->udpSocket = rzUDPSocketNew(port);
    if (!listener->udpSocket) {
        fprintf(stderr, "[ERROR] Failed to start UDP socket\n");
        free(listener);
        return 0;
    }

    listener->maxConnections = maxConnections;
    listener->running = 0;

    return listener;
}

void rzListenerFree(struct RZListener* listener)
{
    rzUDPSocketFree(listener->udpSocket);
    free(listener);
}

int rzListenerListen(struct RZListener* listener)
{
    listener->running = 1;

    while (listener->running) {
        struct RZPacket* packet = rzUDPSocketRecv(listener->udpSocket);
        if (packet->size == -1) {
            fprintf(stderr, "[ERROR] rzUDPSocketRecv failed\n");
            continue;
        }

        char* addrString = (char*)malloc(16);

        inet_ntop(AF_INET, &packet->addr.sin_addr, addrString, 16);
        printf(
                "[INFO] Received packet from %s:%d\n",
                addrString, ntohs(packet->addr.sin_port)
        );
        printf("[INFO] Data: ");
        for(int i = 0; i < packet->size; i++) {
            printf("%c", packet->buffer[i]);
        }
        printf(" (%i bytes)\n", packet->size);

        rzPacketFree(packet);
    }

    return 0;
}

int rzListenerAdjustMaxConnections(struct RZListener* listener, unsigned short maxConnections)
{
    RZClientSocket* newBlock = realloc(listener->clients, maxConnections);
    if (!newBlock) {
        fprintf(stderr, "[ERROR] Clients realloc failed\n");
        return -1;
    }

    listener->maxConnections = maxConnections;
    listener->clients = newBlock;
    return 0;
}