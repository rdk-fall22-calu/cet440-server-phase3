/*
 *@file logging.c
 *@author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
    */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logging.h"

static FILE* logFile;


void initialize_logging()
{
    logFile = fopen(LOG_FILE_NAME, "a");
    log_message("SERVER", "Logging initialized.");
}


void log_message(char *sender, char *message)
{
    // Print the time
    time_t now;
    time(&now);
    struct tm *curTime = localtime(&now);

    // Setup the message
    char printMesssage[256];
    snprintf(printMesssage, sizeof(printMesssage), "[%02d:%02d:%02d] [%s] %s\n", 
        curTime->tm_hour, 
        curTime->tm_min, 
        curTime->tm_sec,
        sender,
        message
    );

    // Print message to console
    printf("%s", printMesssage);

    // Print message to log file
    fprinf(logFile, printMesssage);

}
