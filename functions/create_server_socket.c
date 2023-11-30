#include "../threadbank.h"
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
/**
 * @brief creates server socket; starts socket, unlinks previous, binds socket and starts listening to socket.
 * 
 * @param domain the domain of the socket
 * @param type the type of the socket
 * 
 * @return the socket in question, type int.
*/
int create_server_socket(int domain, int type) {
    int mySocket = socket(domain, type, 0);
    if (mySocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    if (unlink("./socket") == -1 && (errno != ENOENT)) {
        perror("Unlinking socket");
        return -1;
    }

    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = domain;
    strcpy(address.sun_path, "./socket");

    if (bind(mySocket, (struct sockaddr*) &address, sizeof(address)) == -1) {
        perror("Error binding socket");
        close(mySocket);
        return -1;
    }

    if (listen(mySocket, 20) == -1) {
        perror("Error listening on socket");
        close(mySocket);
        return -1;
    }

    return mySocket;
}

