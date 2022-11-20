/**
 * @file logging.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/

#ifndef _LOGGING_H
#define _LOGGING_H

#define LOG_FILE_NAME "log.txt"

/**
 * @brief Sets up the logging module
 * 
 */
void initialize_logging();

/**
 * @brief Log the formatted message to the console
 * 
 * @param sender The sender's ID
 * @param message Message to be logged
 */
void log_message(char *sender, char *message);

#endif
