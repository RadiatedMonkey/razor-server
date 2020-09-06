#include "listener.h"

#include <stdio.h>
#include <stdlib.h>

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
        int receiveSize = rzUDPSocketRecv(listener->udpSocket);
        if (receiveSize == -1) {
            fprintf(stderr, "[ERROR] rzUDPSocketRecv failed\n");
            continue;
        }
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