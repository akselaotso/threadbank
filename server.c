#include "threadbank.h"
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h> 
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

typedef struct {
    int sockets[MAX_SOCKETS];
    int count;
} socketQueue;

int endProgram = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
socketQueue mainQueue[DESK_COUNT];

/**
 * @brief Function to change endProgram parameter, used as signal handler for SIGTERM and SIGINT to enable graceful shutdown.
 * 
 * @return void
*/
void change_endProgram() { endProgram = 1; }

/**
 * @brief creates the log file for logging the servers actions.
 * 
 * @return 1 if error, 0 if success.
*/
int create_log_file() {
    FILE* logfile = fopen("log_file.txt", "w");
    if (logfile == NULL) {
        perror("Error creating logfile.");
        return 1;
    }
    fclose(logfile);
    return 0;
}

/**
 * @brief Gets next socket from mainQueue
 * 
 * @return the next socket
*/
int get_next_socket(int index) {
    LOCK(mutex);
    int first = mainQueue[index].sockets[0];
    for (int i = 0; i < mainQueue[index].count; i++) mainQueue[index].sockets[i] = mainQueue[index].sockets[i + 1];
    UNLOCK(mutex);
    return first;    
}

/**
 * @brief the receiver thread. This thread listens to the socket and directs new arrivals to the main queue on the server.
*/
void* reciever_thread(void* serverSocket_input) {
    int socket = *((int *)serverSocket_input);
    pthread_t id = pthread_self();
    pthread_detach(id);
    printf("Receiver thread running.\n");

    int client;
    while (1) {
        if ((client = accept(socket, NULL, NULL)) == -1 ){
          perror("accept() in receiver_thread()");
          exit(EXIT_FAILURE);
        }
        printf("recieved client %d\n", client);
        LOCK(mutex);

        int minIndex = 0;
        int minCount = 0;
        for (int i = 0; i < DESK_COUNT; i++) {
            if (mainQueue[i].count <= minCount) {
                minIndex = i;
                minCount = mainQueue[i].count;
            }
        }
        
        mainQueue[minIndex].sockets[mainQueue[minIndex].count] = client;
        mainQueue[minIndex].count++;
        UNLOCK(mutex);
    }
}

/**
 * @brief desk thread. Multiple of these can be formed at the launch of the program. These threads take sockets from the socket queue and then start interacting with the client.
*/
void* desk_thread(void* deskNumber_input) {
    int c = *((int *)deskNumber_input);
    int deskNumber = c;

    pthread_t id = pthread_self();
    pthread_detach(id);
    printf("A desk thread %d has started and is now running.\n", deskNumber);
    log_action("A desk thread started");

    int clientSocket;
    while (1) {
        while (mainQueue[deskNumber].count == 0) sleep(1);
        clientSocket = get_next_socket(deskNumber);
        printf("Thread %d serving new client\n", deskNumber);

        char* message = "ready";
        if (send(clientSocket, message, strlen(message), 0) == -1) {
            printf("Desk thread had error sending initial message\n");
            close(clientSocket);
        } else {
            ssize_t bytesRead;
            char recievedMessage[BUFFER_SIZE];
            char response[BUFFER_SIZE]; 
            while ((bytesRead = recv(clientSocket, recievedMessage, sizeof(recievedMessage), 0)) > 0) {
                printf(recievedMessage);
                bank_action(recievedMessage, response);
                printf(response);

                if (send(clientSocket, response, strlen(response), 0) == -1) {
                    printf("Desk thread error sending message");
                    close(clientSocket);
                    break;
                }
                memset(&recievedMessage, 0, sizeof(recievedMessage));
            }
        }
        mainQueue[deskNumber].count -= 1;
        printf("Client exited desk %d\n", deskNumber);

        LOCK(mutex);
        log_action("Client service complete");
        UNLOCK(mutex);
    }
}

int main(int argc, char* argv[]) {
    if (create_log_file() != 0) exit(EXIT_FAILURE);

    int serverSocket = create_server_socket(AF_UNIX, SOCK_STREAM);
    if (serverSocket == -1) exit(EXIT_FAILURE);

    pthread_t reciever;
    if (pthread_create(&reciever, NULL, reciever_thread, (void*)&serverSocket) != 0) {
        perror("pthread_create() for reciever");
        unlink("./socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    pthread_t desks[DESK_COUNT];
    int i = 0;
    while (i < DESK_COUNT) {
        if (pthread_create(&desks[i], NULL, desk_thread, (void*)&i) != 0) {
            perror("pthread_create() for desks");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        i++;
    }

    signal(SIGTERM, change_endProgram);
    signal(SIGINT, change_endProgram);

    printf("[TYPE q AND PRESS ENTER TO QUIT AND END PROCESS]\n");
    while (!endProgram) if (getchar() == 'q') break;

    printf("Exiting, please wait...\n");

    i = 0;
    while (i < DESK_COUNT) {
        if (pthread_cancel(desks[i]) != 0) perror("Cancelling desk thread");
        sleep(1);
        i++;
    }

    if (pthread_cancel(reciever) != 0) perror("Cancelling reciever thread");

    close(serverSocket);
    unlink("./socket");
    return 0;
}

