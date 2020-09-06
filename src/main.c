#include <stdio.h>

#include "listener.h"

int main(void) {
    struct Listener* listener = listenerNew(19132, 250);
    if (!listener) {
        fprintf(stderr, "Failed to start listener on port %d\n", 19132);
        return 1;
    }

    int returnCode = listenerListen(listener);

    listenerFree(listener);
    return returnCode;
}