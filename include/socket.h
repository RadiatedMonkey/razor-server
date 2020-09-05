#ifndef RZ_SOCKET_H
#define RZ_SOCKET_H

#define RECEIVE_BUFFER_SIZE 1500

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

struct UDPSocket {
    unsigned short port;
    char buffer[RECEIVE_BUFFER_SIZE];
    WSADATA wsa;
    SOCKET netSocket; // socket is already used by a winsock2 function
    struct sockaddr_in sender;
    int senderLength;
};
#else
#include <arpa/inet.h>
#include <sys/socket.h>

struct UDPSocket {
    unsigned short port;
    char buffer[RECEIVE_BUFFER_SIZE];
    int netSocket;
    struct sockaddr_in sender;
    int senderLength;
};
#endif

// Creates a new UDPSocket and initializes everything
struct UDPSocket* rzUDPSocketNew(unsigned short port);

// UDPSocket->buffer contains the data and the return value contains the length of received data
int rzUDPSocketRecv(struct UDPSocket* udpSocket);

// Sends a specified amount of bytes from buffer to the recipient
int rzUDPSocketSend(struct UDPSocket* udpSocket, const char* buffer, int bufferSize, struct sockaddr* recipient);

// Destroys created objects and frees all memory
void rzUDPSocketFree(struct UDPSocket* udpSocket);

#endif