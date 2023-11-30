#include "../threadbank.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>

/**
 * @brief reads the accounts balance when provided with the accounts "FILE*" file address.
 * 
 * @param file type FILE* and should be the file from which the balance is read.
 * 
 * @return the balance in the account, stored in the file provided.
*/
int read_balance(int account) {
    char filename[BUFFER_SIZE]; 
    sprintf(filename, "accounts/%d", account);
    FILE* file = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    int bytesread = fread(buffer, 1, sizeof(buffer), file);
    if (bytesread == -1) {
        fclose(file);
        printf("Error reading file.");
        return -32767;
    }

    fclose(file);
    return atoi(buffer);
}

/**
 * @brief opens a file with specified details. If file is locket waits until the lock is lifted to lock it again.
 * 
 * @param account the account number for which the file should be opened.
 * @param arguments the arguments for opening the file e.g. "r" or "w"
 * 
 * @return reference to the file, type FILE*
*/
FILE* rwlock_open_file(int account, char* arguments) {
    char filename[BUFFER_SIZE]; 
    sprintf(filename, "accounts/%d", account);

    FILE* file = fopen(filename, arguments);
    if (file == NULL) return NULL;

    while (flock(fileno(file), LOCK_EX) == -1) {
        if (errno != EWOULDBLOCK) return NULL;
    }

    return file;
}

/**
 * @brief unlocks and closes specified file
 * 
 * @param file the file to be closed
 * 
 * @return 0 if success, -1 if failed.
*/
int rwlock_close_file(FILE* file) {
    if (flock(fileno(file), LOCK_UN) == -1) {
        perror("flock(bankfile, LOCK_UN)");
        return -1;
    }
    fclose(file);
    return 0;
}

