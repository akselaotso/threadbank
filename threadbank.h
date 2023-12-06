#ifndef THREADBANK_H
#define THREADBANK_H

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
#include <sys/socket.h>
#include <sys/file.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_SOCKETS 100
#define BUFFER_SIZE 180
#define DESK_COUNT 10

typedef struct {
    int sockets[MAX_SOCKETS];
    int count;
} socketQueue;

#define LOCK(mutex) pthread_mutex_lock(&mutex)
#define UNLOCK(mutex) pthread_mutex_unlock(&mutex)


/** BANK ACTIONS DIRECTORY FUNCTIONS */

/**
 * @brief calls the appropriate function for each command and returns the result.
 * 
 * @param message - the message / command which should be of format "[command char] [command parameters]"
 * 
 * @return string which contains the server response to the given command
*/
void bank_action(char* message, char* response);

/**
 * @brief Completes the "d" command aka deposits the specified amount to an account. If account exists (checked with "access()") increments existing funds. If account does not exist function creates the account. 
 * 
 * @param message - the message / command which should be of format "d [account number] [amount to deposit]".
 * 
 * @return string which contains the server response to the given command
*/
void d_command(char* message, char* response);

/**
 * @brief Completes the "t" command aka returns transfers a specified amount of "money" from one specified account to another. Returns fail if both accounts don't exist.
 * 
 * @param message - the message / command which should be of format "l [account number]".
 * 
 * @return string which contains the server response to the given command
*/
void t_command(char* message, char* response);

/**
 * @brief Completes the "w" command aka withdraws the specified amount of "money" from the sepcified account. Returns fail if account does not exist.
 * 
 * @param message - the message / command which should be of format "w [account number] [amount to withdraw]".
 * 
 * @return string which contains the server response to the given command
*/
void w_command(char* message, char* response);

/**
 * @brief Completes the "l" command aka returns the balance stored in the specified account. Returns fail if account does not exist.
 * 
 * @param message - the message / command which should be of format "l [account number]".
 * 
 * @return string which contains the server response to the given command
*/
void l_command(char* message, char* response);


/** FUNCTIONS DIRECTORY FUNCTIONS */

/**
 * @brief reads the accounts balance when provided with the accounts "FILE*" file address.
 * 
 * @param file type FILE* and should be the file from which the balance is read.
 * 
 * @return the balance in the account, stored in the file provided.
*/
int read_balance(int account);

/**
 * @brief writes the given text, representing the action, to the log file.
 * 
 * @file functions/log_action.c
 * 
 * @param text the text which will be written
 * 
 * @return 1 if the process failed, 0 if the process was a success
*/
int log_action(char* text);

/**
 * @brief creates server socket; starts socket, unlinks previous, binds socket and starts listening to socket.
 * 
 * @param domain the domain of the socket
 * @param type the type of the socket
 * 
 * @return the socket in question, type int.
*/
int create_server_socket(int domain, int type);

/**
 * @brief opens a file with specified details. If file is locket waits until the lock is lifted to lock it again.
 * 
 * @param account the account number for which the file should be opened.
 * @param arguments the arguments for opening the file e.g. "r" or "w"
 * 
 * @return reference to the file, type FILE*
*/
FILE* rwlock_open_file(int account, char* arguments);

/**
 * @brief unlocks and closes specified file
 * 
 * @param file the file to be closed
 * 
 * @return 0 if success, -1 if failed.
*/
int rwlock_close_file(FILE* file);

/**
 * @brief Function to change endProgram parameter, used for signaln handlers for SIGTERM and SIGINT to enable graceful shutdown.
 * 
 * @return void
*/
void change_endProgram();

/**
 * @brief creates the log file for logging the servers actions.
 * 
 * @return 1 if error, 0 if success.
*/
int create_log_file();

/** SERVER.C FILE FUNCTIONS */

/**
 * @brief Gets next socket from mainQueue
 * 
 * @return the next socket
*/
int get_next_socket();

/**
 * @brief the receiver thread. This thread listens to the socket and directs new arrivals to the main queue on the server.
*/
void* receiver_thread(void* serverSocket_input);

/**
 * @brief desk thread. Multiple of these can be formed at the launch of the program. These threads take sockets from the socket queue and then start interacting with the client.
*/
void* desk_thread(void* deskNumber_input);


#endif

