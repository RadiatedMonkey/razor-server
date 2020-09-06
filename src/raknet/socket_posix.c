#include "raknet/socket.h"

#include <stdio.h>
#include <stdlib.h>

struct RZUDPSocket* rzUDPSocketNew(unsigned short port)
{
    struct RZUDPSocket* udpSocket = (struct RZUDPSocket*)malloc(sizeof(struct RZUDPSocket));
    if (!udpSocket) {
        fprintf(stderr, "[ERROR] UDPSocket memory allocation failure\n");
        return 0;
    }

    udpSocket->senderLength = sizeof(udpSocket->sender);

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

void rzUDPSocketFree(struct RZUDPSocket* udpSocket)
{
    close(udpSocket->netSocket);
    free(udpSocket);
}

int rzUDPSocketRecv(struct RZUDPSocket* udpSocket)
{
    int receiveSize = recvfrom(
        udpSocket->netSocket, udpSocket->buffer, RECEIVE_BUFFER_SIZE,
        0, (struct sockaddr*)&udpSocket->sender, &udpSocket->senderLength
    );
    if (receiveSize == -1) {
        fprintf(stderr, "[ERROR] Failed to receive packet from sender\n");
        return 0;
    }

    return receiveSize;
}

int rzUDPSocketSend(struct RZUDPSocket* udpSocket, const char* buffer, int bufferSize, struct sockaddr* recipient)
{
    int sendResult = sendto(udpSocket->netSocket, buffer, bufferSize, 0, recipient, sizeof(struct sockaddr));
    if (sendResult == -1) {
        fprintf(stderr, "[ERROR] Failed to send packet to recipient\n");
        return -1;
    }

    return 0;
}