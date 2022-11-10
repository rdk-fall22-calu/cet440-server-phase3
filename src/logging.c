/*
 *@file logging.c
 *@author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
    */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void log_message(char *sender, char *message)
{
    // Print the time
    time_t now;
    time(&now);
    struct tm *curTime = localtime(&now);
    printf("[%02d:%02d:%02d] ", curTime->tm_hour, curTime->tm_min, curTime->tm_sec);

    // Print the sender info
    printf("[%s] ", sender);

    // Print the message
    printf("%s", message);
    
    // Newline
    printf("\n");

}
