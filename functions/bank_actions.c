#include "../threadbank.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief calls the appropriate function for each command and returns the result.
 * 
 * @param message - the message / command which should be of format "[command char] [command parameters]"
 * 
 * @return string which contains the server response to the given command
*/
void bank_action(char* message, char* response) {
    char command = message[0];
    if (command == 'l') l_command(message, response);
    else if (command == 'w') w_command(message, response);
    else if (command == 't') t_command(message, response);
    else if (command == 'd') d_command(message, response);
    else strcpy(response, "fail: command does not exist");
}

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

