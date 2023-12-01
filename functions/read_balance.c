#include "../threadbank.h"

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
    if (file == NULL) return -32767;
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

