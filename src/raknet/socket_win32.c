#include "raknet/socket.h"

#include <stdio.h>
#include <stdlib.h>

struct RZUDPSocket* rzUDPSocketNew(unsigned short port)
{
    struct RZUDPSocket* udpSocket = (struct RZUDPSocket*)malloc(sizeof(struct RZUDPSocket));
    if (!udpSocket) {
        fprintf(stderr, "[ERROR] UDPSocket malloc failed\n");
        return 0;
    }

    int iResult = WSAStartup(MAKEWORD(2, 2), &udpSocket->wsa);
    if (iResult != 0) {
        fprintf(stderr, "[ERROR] WSAStartup failed %d\n", iResult);
        free(udpSocket);
        return 0;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "[ERROR] Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        free(udpSocket);
        return 0;
    }
    udpSocket->netSocket = sock;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int bindResult = bind(udpSocket->netSocket, (struct sockaddr*)&server, sizeof(server));
    if (bindResult == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] Socket binding failed: %d\n", WSAGetLastError());
        closesocket(udpSocket->netSocket);
        WSACleanup();
        free(udpSocket);
        return 0;
    }

    printf("[INFO] Running on port %d\n", port);

    return udpSocket;
}

void rzUDPSocketFree(struct RZUDPSocket* udpSocket)
{
    closesocket(udpSocket->netSocket);
    WSACleanup();
    free(udpSocket);
}

struct RZPacket* rzUDPSocketRecv(struct RZUDPSocket* udpSocket)
{
    struct RZPacket* packet = rzPacketNew();
    if (!packet) {
        fprintf(stderr, "[ERROR] Failed to initialize packet\n");
        return 0;
    }

    int receiveSize = recvfrom(
            udpSocket->netSocket, udpSocket->buffer, RECEIVE_BUFFER_SIZE,
            0, (struct sockaddr*)&packet->addr, &packet->addrSize
    );
    if (receiveSize == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] recvfrom failed: %d\n", WSAGetLastError());
        free(packet);
        return 0;
    }

    packet->buffer = (char*)malloc(receiveSize);
    if (!packet->buffer) {
        fprintf(stderr, "[ERROR] packet buffer malloc failed\n");
        free(packet);
        return 0;
    }

    packet->size = receiveSize;
    memcpy(packet->buffer, udpSocket->buffer, receiveSize);

    return packet;
}

int rzUDPSocketSend(struct RZUDPSocket* udpSocket, struct RZPacket* packet)
{
    int sendResult = sendto(
            udpSocket->netSocket, packet->buffer, packet->size,
            0, (struct sockaddr*)&packet->addr, sizeof(struct sockaddr)
    );
    if (sendResult == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] sendto failed: %d\n", WSAGetLastError());
        return -1;
    }

    return 0;
}