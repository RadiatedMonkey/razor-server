#ifndef RZ_SERVER_H
#define RZ_SERVER_H

#include "raknet/socket.h"

#include <winsock2.h>

#ifdef WIN32
typedef unsigned int ClientSocket;
#else
typedef int ClientSocket;
#endif

struct Listener {
    struct UDPSocket* udpSocket;
    unsigned short maxConnections;
    _Bool running;
    ClientSocket* clients;
    const char* pongData;
};

/*
 *  listenerNew
 *  @brief  Creates a new listener
 *
 *  @details This function creates a new listener that will run on the specified port and only accept a specified
 *  amount of connections, use listenerListen to start the listener
 */
struct Listener* listenerNew(unsigned short port, unsigned short maxConnections, const char* pongData);

/*
 *  listenerFree
 *  @brief  Frees all memory of the specified listener
 *
 *  @details Make sure to call listenerWaitUntilShutdown before calling this to not free the terminate
 *  the server while it's disconnecting everyone
 */
void listenerFree(struct Listener* listener);

/*
 *  listenerShutdown
 *  @brief  Shuts the listener down gracefully and kicks all connected clients
 */
void listenerShutdown(struct Listener* listen);

/*
 *  listenerWaitUntilShutdown
 *  @brief  Waits until the listener has fully shutdown
 *
 *  @details This function should be called before free to be sure
 *  that the server has fully shutdown
 */
void listenerWaitUntilShutdown(struct Listener* listener);

/*
 *  listenerListen
 *  @brief  Starts the listener created with rzListenerNew
 *
 *  @details This function returns 0 on success and -1 when an error occurs
 */
int listenerListen(struct Listener* listener);

/*
 *  listenerAdjustMaxConnections
 *  @brief  Changes the max amount of connections
 *
 *  @details This function changes the max amount of connections the listener will accept
 *  and resizes the clients array to be able to fit the specified amount of clients
 */
int listenerAdjustMaxConnections(struct Listener* listener, unsigned short maxConnections);

#endif // RZ_SERVER_H