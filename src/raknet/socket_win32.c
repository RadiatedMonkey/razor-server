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

    udpSocket->senderLength = sizeof(udpSocket->sender);

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

int rzUDPSocketRecv(struct RZUDPSocket* udpSocket)
{
    int receiveSize = recvfrom(
            udpSocket->netSocket, udpSocket->buffer, RECEIVE_BUFFER_SIZE,
            0, (struct sockaddr*)&udpSocket->sender, &udpSocket->senderLength
    );
    if (receiveSize == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] recvfrom failed: %d\n", WSAGetLastError());
        return 0;
    }

    printf(
            "[INFO] Received packet from %s:%d\n",
            inet_ntoa(udpSocket->sender.sin_addr), ntohs(udpSocket->sender.sin_port)
    );
    printf("[INFO] Data: ");
    for(int i = 0; i < receiveSize; i++) {
        printf("%c", udpSocket->buffer[i]);
    }
    printf(" (%i bytes)\n", receiveSize);

    return receiveSize;
}

int rzUDPSocketSend(struct RZUDPSocket* udpSocket, const char* buffer, int bufferSize, struct sockaddr* recipient)
{
    int sendResult = sendto(udpSocket->netSocket, buffer, bufferSize, 0, recipient, sizeof(struct sockaddr));
    if (sendResult == SOCKET_ERROR) {
        fprintf(stderr, "[ERROR] sendto failed: %d\n", WSAGetLastError());
        return -1;
    }

    printf("[INFO] Sent %d bytes to %s:%d\n", bufferSize, inet_ntoa(udpSocket->sender.sin_addr), ntohs(udpSocket->sender.sin_port));

    return 0;
}