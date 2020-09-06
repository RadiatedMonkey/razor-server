#include <stdio.h>

#include "listener.h"

int main(void) {
    struct RZListener* listener = rzListenerNew(19132, 250);
    if (!listener) {
        fprintf(stderr, "Failed to start listener on port %d\n", 19132);
        return 1;
    }

    int returnCode = rzListenerListen(listener);

    rzListenerFree(listener);
    return returnCode;
}