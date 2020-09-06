#ifndef RZ_SERVER_H
#define RZ_SERVER_H

#include "raknet/socket.h"

#include <winsock2.h>

#ifdef WIN32
typedef unsigned int RZClientSocket;
#else
typedef int RZClientSocket;
#endif

struct RZListener {
    struct RZUDPSocket* udpSocket;
    unsigned short maxConnections;
    _Bool running;
    RZClientSocket* clients;
};

/*
 *  rzListenerNew
 *  @brief  Creates a new listener
 *
 *  @details This function creates a new listener that will run on the specified port and only accept a specified
 *  amount of connections, use rzListenerListen to start the listener
 */
struct RZListener* rzListenerNew(unsigned short port, unsigned short maxConnections);

/*
 *  rzListenerFree
 *  @brief  Frees all memory of the specified listener
 *
 *  @details Make sure to call rzListenerWaitUntilShutdown before calling this to not free the terminate
 *  the server while it's disconnecting everyone
 */
void rzListenerFree(struct RZListener* listener);

/*
 *  rzListenerShutdown
 *  @brief  Shuts the listener down gracefully and kicks all connected clients
 */
void rzListenerShutdown(struct RZListener* listen);

/*
 *  rzListenerWaitUntilShutdown
 *  @brief  Waits until the listener has fully shutdown
 *
 *  @details This function should be called before free to be sure
 *  that the server has fully shutdown
 */
void rzListenerWaitUntilShutdown(struct RZListener* listener);

/*
 *  rzListenerListen
 *  @brief  Starts the listener created with rzListenerNew
 *
 *  @details This function returns 0 on success and -1 when an error occurs
 */
int rzListenerListen(struct RZListener* listener);

/*
 *  rzListenerAdjustMaxConnections
 *  @brief  Changes the max amount of connections
 *
 *  @details This function changes the max amount of connections the listener will accept
 *  and resizes the clients array to be able to fit the specified amount of clients
 */
int rzListenerAdjustMaxConnections(struct RZListener* listener, unsigned short maxConnections);

#endif // RZ_SERVER_H