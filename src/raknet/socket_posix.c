#include "raknet/socket.h"

#include <stdio.h>
#include <stdlib.h>

struct UDPSocket* UDPSocketNew(unsigned short port)
{
    struct UDPSocket* udpSocket = (struct UDPSocket*)malloc(sizeof(struct UDPSocket));
    if (!udpSocket) {
        fprintf(stderr, "[ERROR] UDPSocket malloc failed\n");
        return 0;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        fprintf(stderr, "[ERROR] Socket creation failed\n");
        free(udpSocket);
        return 0;
    }
    udpSocket->netSocket = sock;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    int bindResult = bind(udpSocket->netSocket, (struct sockaddr*)&server, sizeof(server));
    if (bindResult == -1) {
        fprintf(stderr, "[ERROR] Socket binding failed\n");
        close(udpSocket->netSocket);
        free(udpSocket);
        return 0;
    }

    printf("[INFO] Running on port %d\n", port);

    return udpSocket;
}

void UDPSocketFree(struct UDPSocket* udpSocket)
{
    close(udpSocket->netSocket);
    free(udpSocket);
}

struct Packet* UDPSocketRecv(struct UDPSocket* udpSocket)
{
    struct Packet* packet = packetNew();
    if (!packet) {
        fprintf(stderr, "[ERROR] Failed to initialize packet\n");
        return 0;
    }

    int receiveSize = recvfrom(
        udpSocket->netSocket, udpSocket->buffer, RECEIVE_BUFFER_SIZE,
        0, (struct sockaddr*)&packet->addr, &packet->addrSize
    );
    if (receiveSize == -1) {
        fprintf(stderr, "[ERROR] Failed to receive packet from sender\n");
        return 0;
    }

    packet->buffer = (char*)malloc(receiveSize);
    if (!packet->buffer) {
        fprintf(stderr, "[ERROR] Packet buffer malloc failed\n");
        free(packet);
        return 0;
    }

    packet->size = receiveSize;
    memcpy(packet->buffer, udpSocket->buffer, receiveSize);

    return packet;
}

int UDPSocketSend(struct UDPSocket* udpSocket, struct Packet* packet)
{
    int sendResult = sendto(
        udpSocket->netSocket, packet->buffer, packet->size,
        0, (struct sockaddr*)&packet->addr, sizeof(struct sockaddr)
    );
    if (sendResult == -1) {
        fprintf(stderr, "[ERROR] Failed to send packet\n");
        return -1;
    }

    return 0;
}