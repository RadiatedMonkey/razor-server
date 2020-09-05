#ifndef RZ_SOCKET_H
#define RZ_SOCKET_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

struct UDPSocket {
    unsigned short port;
    WSADATA wsa;
    SOCKET sock;
};
#else
struct UDPSocket {
    unsigned short port;
};
#endif

struct UDPSocket* rzNewUDPSocket(unsigned short port);
void rzFreeUDPSocket(struct UDPSocket* udpSocket);

#endif