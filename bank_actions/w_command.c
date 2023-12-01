#include "../threadbank.h"

/**
 * @brief Completes the "w" command aka withdraws the specified amount of "money" from the sepcified account. Returns fail if account does not exist.
 * 
 * @param message - the message / command which should be of format "w [account number] [amount to withdraw]".
 * 
 * @return string which contains the server response to the given command
*/
void w_command(char* message, char* response) {
    int account; int amount;
    if (sscanf(message,"w %d %d", &account, &amount) == 2) {
        int balance = read_balance(account);
        if (balance == -32767) sprintf(response, "fail: Error reading balance\n");
        else {
            balance -= amount;
            FILE* accountFile = rwlock_open_file(account, "w");
            if (accountFile == NULL) sprintf(response, "fail: Error opening file, check the account number.\n");
            else {
                fseek(accountFile, 0, SEEK_SET);
                fprintf(accountFile, "%d", balance);

                if (rwlock_close_file(accountFile) != 0) sprintf(response, "Error closing file\n"); 
                else sprintf(response, "ok: Account %d now has balance %d\n", account, balance);   
            }
        }
    } 
    else sprintf(response, "fail: Wrong use of w; correct use 'w <account> <amount>'.\n");
}

