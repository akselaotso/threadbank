#include "../threadbank.h"

/**
 * @brief creates the log file for logging the servers actions.
 * 
 * @return 1 if error, 0 if success.
*/
int create_log_file() {
    FILE* logfile = fopen("log_file.txt", "w");
    if (logfile == NULL) {
        perror("Error creating logfile.");
        return 1;
    }
    fclose(logfile);
    return 0;
}

