#include "../threadbank.h"

/**
 * @brief Completes the "t" command aka returns transfers a specified amount of "money" from one specified account to another. Returns fail if both accounts don't exist.
 * 
 * @param message - the message / command which should be of format "l [account number]".
 * 
 * @return string which contains the server response to the given command
*/
void t_command(char* message, char* response) {
    int sender; int reciever; int amount;
    if (sscanf(message,"t %d %d %d", &sender, &reciever, &amount) == 3) {
        int senderBalance = read_balance(sender);
        int recieverBalance = read_balance(reciever);
        if ((recieverBalance == -32767) || (senderBalance == -32767)) sprintf(response, "fail: Error reading balance\n");
        else {
            FILE* senderAccountFile = rwlock_open_file(sender, "w");
            FILE* recieverAccountFile = rwlock_open_file(reciever, "w");
            if ((recieverAccountFile == NULL) || (senderAccountFile == NULL)) sprintf(response, "fail: Error opening file, check the account number.\n");                
            else {
                recieverBalance += amount;
                senderBalance -= amount;

                fseek(senderAccountFile, 0, SEEK_SET);
                fprintf(senderAccountFile, "%d", senderBalance);
                fseek(recieverAccountFile, 0, SEEK_SET);
                fprintf(recieverAccountFile, "%d", recieverBalance);

                if ((rwlock_close_file(senderAccountFile) != 0) || (rwlock_close_file(recieverAccountFile) != 0)) sprintf(response, "Error closing file\n"); 
                else sprintf(response, "ok: Account %d has transferred %d to account number %d\n", sender, amount, sender);
            }
        }
    }
    else sprintf(response, "fail: Wrong use of t; correct use 't <sender> <reciever> <amount>'.\n");
}

