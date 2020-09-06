#ifndef RZ_SOCKET_H
#define RZ_SOCKET_H

#define RECEIVE_BUFFER_SIZE 1500

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

struct RZUDPSocket {
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

struct RZUDPSocket {
    unsigned short port;
    char buffer[RECEIVE_BUFFER_SIZE];
    int netSocket;
    struct sockaddr_in sender;
    int senderLength;
};
#endif

/*
 *  rzUDPSocketNew
 *  @brief  Creates a new cross-platform UDP socket on the specified port
 *
 *  @details (This function is only used internally, you shouldn't have to use this)
 */
struct RZUDPSocket* rzUDPSocketNew(unsigned short port);

/*
 *  rzUDPSocketRecv
 *  @brief  Receives a packet from a client.
 *
 *  @details This function returns the amount of bytes received and stores the data in the buffer field
 *  of the RZUDPSocket struct.
 *  (This function is only used internally, you shouldn't have to use this)
 */
int rzUDPSocketRecv(struct RZUDPSocket* udpSocket);

/*
 *  rzUDPSocketSend
 *  @brief Sends data to a specified client
 *
 *  @details  Sends bufferSize bytes from buffer to the specified recipient.
 *  This functions returns 0 on success and -1 when an error occurred.
 *  (This function is only used internally, you shouldn't have to use this)
 */
int rzUDPSocketSend(struct RZUDPSocket* udpSocket, const char* buffer, int bufferSize, struct sockaddr* recipient);

/*
 *  rzUDPSocketFree
 *  @brief  Frees the entire RZUDPSocket struct
 *
 *  @details (This functions is used internally, you shouldn't have to use this)
 */
void rzUDPSocketFree(struct RZUDPSocket* udpSocket);

#endif