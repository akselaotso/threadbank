#include "../threadbank.h"

/**
 * @brief unlocks and closes specified file
 * 
 * @param file the file to be closed
 * 
 * @return 0 if success, -1 if failed.
*/
int rwlock_close_file(FILE* file) {
    if (flock(fileno(file), LOCK_UN) == -1) {
        perror("flock(bankfile, LOCK_UN)");
        return -1;
    }
    fclose(file);
    return 0;
}

