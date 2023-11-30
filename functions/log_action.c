#include "../threadbank.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @brief writes the given text, representing the action, to the log file.
 * 
 * @param text the text which will be written
 * 
 * @return 1 if the process failed, 0 if the process was a success
*/
int log_action(char* text) {
    // FILE* outputFile = fopen("log_file.txt", "a");
    // if (outputFile == NULL) {
    //     return 1;
    // }

    // time_t t;
    // time(&t);
    // struct tm* myTime = localtime(&t);
    // char timeString[50];
    // strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", myTime);
    
    // fprintf(outputFile, "%s // %s\n", timeString, text);
    // fclose(outputFile);
    return 0;
}

