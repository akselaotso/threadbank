#include "../threadbank.h"

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