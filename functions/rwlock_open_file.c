#include "../threadbank.h"

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

