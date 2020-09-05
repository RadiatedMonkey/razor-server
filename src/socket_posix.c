#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

struct UDPSocket* rzUDPSocketNew(unsigned short port)
{
    struct UDPSocket* udpSocket = (struct UDPSocket*)malloc(sizeof(struct UDPSocket));
    if (!udpSocket) {
        fprintf(stderr, "ERROR: UDPSocket memory allocation failure\n");
        return 0;
    }

    udpSocket->senderLength = sizeof(udpSocket->sender);

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        fprintf(stderr, "ERROR: Socket creation failed\n");
        free(udpSocket);
        return 0;
    }
    udpSocket->netSocket = sock;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int bindResult = bind(udpSocket->netSocket, (struct sockaddr*)&server, sizeof(server));
    if (bindResult == -1) {
        fprintf(stderr, "ERROR: Socket binding failed\n");
        closesocket(udpSocket->netSocket);
        free(udpSocket);
        return 0;
    }

    printf("INFO: Running on port %d\n", port);

    return udpSocket;
}

void rzUDPSocketFree(struct UDPSocket* udpSocket)
{
    closesocket(udpSocket->netSocket);
    free(udpSocket);
}

int rzUDPSocketRecv(struct UDPSocket* udpSocket)
{
    int receiveSize = recvfrom(
        udpSocket->netSocket, udpSocket->buffer, RECEIVE_BUFFER_SIZE,
        0, (struct sockaddr*)&udpSocket->sender, &udpSocket->senderLength
    );
    if (receiveSize == -1) {
        fprintf(stderr, "Failed to receive packet from sender\n");
        return 0;
    }

    return receiveSize;
}

int rzUDPSocketSend(struct UDPSocket* udpSocket, const char* buffer, int bufferSize, struct sockaddr* recipient)
{
    int sendResult = sendto(udpSocket->netSocket, buffer, bufferSize, 0, recipient, sizeof(struct sockaddr));
    if (sendResult == -1) {
        fprintf(stderr, "Failed to send packet to recipient\n");
        return 1;
    }

    return 0;
}