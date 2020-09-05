#include <stdio.h>

#include "socket.h"

int main(void) {
    struct UDPSocket* sock = rzNewUDPSocket(19132);
    if (!sock) {
        fprintf(stderr, "Failed to create UDP socket\n");
        return 1;
    }

    rzFreeUDPSocket(sock);
    return 0;
}