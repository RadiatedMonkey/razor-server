#include <stdio.h>

#include "socket.h"

int main(void) {
    struct UDPSocket* udpSocket = rzUDPSocketNew(19132);
    if (!udpSocket) {
        fprintf(stderr, "Failed to create UDP socket\n");
        return 1;
    }

    while(1) {
        int receiveSize = rzUDPSocketRecv(udpSocket);

        if (!receiveSize) break;
    }

    rzUDPSocketFree(udpSocket);
    return 0;
}