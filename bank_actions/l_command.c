#include "../threadbank.h"

/**
 * @brief Completes the "l" command aka returns the balance stored in the specified account. Returns fail if account does not exist.
 * 
 * @param message - the message / command which should be of format "l [account number]".
 * 
 * @return string which contains the server response to the given command
*/
void l_command(char* message, char* response) {
    int account;
    if (sscanf(message,"l %d",&account) == 1) {
        int balance = read_balance(account);
        if (balance == -32767) sprintf(response, "fail: Error reading balance\n");
        else sprintf(response, "ok: Account %d has balance %d\n", account, balance);
    } 
    else sprintf(response, "fail: Wrong use of l; correct use 'l <account>'.\n");
}

