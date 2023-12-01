#include "../threadbank.h"

/**
 * @brief Completes the "d" command aka deposits the specified amount to an account. If account exists (checked with "access()") increments existing funds. If account does not exist function creates the account. 
 * 
 * @param message - the message / command which should be of format "d [account number] [amount to deposit]".
 * 
 * @return string which contains the server response to the given command
*/
void d_command(char* message, char* response) {
    int account; int amount;
    if (sscanf(message,"d %d %d",&account, &amount) == 2) {
        char filename[100];
        sprintf(filename, "accounts/%d", account);
        
        if (access(filename, F_OK) == 0) {
            int balance = read_balance(account);
            FILE* accountFile = rwlock_open_file(account, "w");
            if (balance == -32767) sprintf(response, "fail: Error reading balance\n");
            else {
                balance += amount;
                fseek(accountFile, 0, SEEK_SET);
                fprintf(accountFile, "%d", balance);

                if (rwlock_close_file(accountFile) != 0) sprintf(response, "Error closing file\n");
                else sprintf(response, "ok: Account %d now has balance %d\n", account, balance);
            }
        }
        else {
            FILE* accountFile = rwlock_open_file(account, "w");
            fprintf(accountFile, "%d", amount);

            if (rwlock_close_file(accountFile) != 0) sprintf(response, "Error closing file\n");
            else sprintf(response, "ok: Account %d now has balance %d\n", account, amount);
        }
    }
    else sprintf(response, "fail: Wrong use of d; correct use 'd <account> <amount>'.\n");
}

