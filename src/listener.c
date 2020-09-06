#include "listener.h"

#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>

struct Listener* listenerNew(unsigned short port, unsigned short maxConnections)
{
    struct Listener* listener = (struct Listener*)malloc(sizeof(struct Listener));
    if (!listener) {
        fprintf(stderr, "[ERROR] RZListener malloc failed\n");
        return 0;
    }

    listener->clients = (ClientSocket*)malloc(sizeof(ClientSocket) * maxConnections);
    if (!listener->clients) {
        fprintf(stderr, "[ERROR] Clients malloc failed\n");
        free(listener);
        return 0;
    }

    listener->udpSocket = UDPSocketNew(port);
    if (!listener->udpSocket) {
        fprintf(stderr, "[ERROR] Failed to start UDP socket\n");
        free(listener);
        return 0;
    }

    listener->maxConnections = maxConnections;
    listener->running = 0;

    return listener;
}

void listenerFree(struct Listener* listener)
{
    UDPSocketFree(listener->udpSocket);
    free(listener);
}

int listenerListen(struct Listener* listener)
{
    listener->running = 1;

    while (listener->running) {
        struct Packet* packet = UDPSocketRecv(listener->udpSocket);
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

        packetFree(packet);
    }

    return 0;
}

int listenerAdjustMaxConnections(struct Listener* listener, unsigned short maxConnections)
{
    ClientSocket* newBlock = realloc(listener->clients, maxConnections);
    if (!newBlock) {
        fprintf(stderr, "[ERROR] Clients realloc failed\n");
        return -1;
    }

    listener->maxConnections = maxConnections;
    listener->clients = newBlock;
    return 0;
}