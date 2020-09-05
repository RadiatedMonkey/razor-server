#include "socket.h"

#include <stdio.h>
#include <stdlib.h>

struct UDPSocket* rzNewUDPSocket(unsigned short port)
{
    struct UDPSocket* udpSocket = (struct UDPSocket*)malloc(sizeof(struct UDPSocket));
    if (!udpSocket) {
        fprintf(stderr, "UDPSocket malloc failed\n");
        return 0;
    }

    int iResult = WSAStartup(MAKEWORD(2u, 2u), &udpSocket->wsa);
    if (iResult != 0) {
        fprintf(stderr, "WSAStartup failed %d\n", iResult);
        free(udpSocket);
        return 0;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        free(udpSocket);
        return 0;
    }
    udpSocket->sock = sock;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    int bindResult = bind(udpSocket->sock, (struct sockaddr*)&server, sizeof(server));
    if (bindResult == SOCKET_ERROR) {
        fprintf(stderr, "Socket binding failed: %d\n", WSAGetLastError());
        closesocket(udpSocket->sock);
        WSACleanup();
        free(udpSocket);
        return 0;
    }

    return udpSocket;
}

void rzFreeUDPSocket(struct UDPSocket* udpSocket)
{
    closesocket(udpSocket->sock);
    WSACleanup();
    free(udpSocket);
}