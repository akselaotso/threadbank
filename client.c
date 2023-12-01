#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <errno.h>

#define BUFFER_SIZE 1020

/**
 * @brief The client function creates a socket and then sends commands to the server, each time listening for the response and printing it to the client to see in stdout.
*/
int main(int argc, char** argv) {
    setvbuf(stdin,NULL,_IOLBF,0);
    setvbuf(stdout,NULL,_IOLBF,0);

    printf("Connecting to the bank, please wait.\n");

    int clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "./socket");

    if (connect(clientSocket, (struct sockaddr*)&address, sizeof(address)) == -1) {
        perror("Error connecting to server socket");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char textBuffer[BUFFER_SIZE];
    if (recv(clientSocket, textBuffer, sizeof(textBuffer), 0) == -1) {
        printf("Server not operational.");
        return 0;
    }

    printf(textBuffer);
    
    char* buf = malloc(BUFFER_SIZE);
    if (buf == NULL) return -1;
    char responseBuffer[BUFFER_SIZE];
    while (1) {
        printf("Next line: \n");
        if (fgets(buf, BUFFER_SIZE, stdin) == NULL) break;

        printf("Recieved command: \n");
        printf(buf);
        char command = buf[0];
        if (command == 'q') break;
        else if ((command == 'l') || (command == 'w') || (command == 't') || (command == 'd')) {
            if (send(clientSocket, buf, strlen(buf), 0) == -1) {
                perror("Error sending message");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }
            memset(&responseBuffer, 0, sizeof(responseBuffer));
            if (recv(clientSocket, responseBuffer, sizeof(responseBuffer), 0) == -1) {
                printf("Server no longer operational.");
                break;
            }

            printf(responseBuffer);
        }
        else printf("fail: Unknown command '%c'.\n", command);
    }

    close(clientSocket);
    free(buf);
    return 0;
}