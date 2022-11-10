/**
 * @file commands.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief 
 * 
 */


#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#define CMD_HELP "HELP"
#define CMD_QUIT "QUIT"
#define CMD_REGISTER "REGISTER"
#define CMD_MYINFO "MYINFO"
#define CMD_ONLINE_USERS "ONLINEUSERS"
#define CMD_REGISTERED_USERS "REGISTEREDUSERS"

/**
 * @brief Returns a string with all of the help information
 * 
 * @return char* Response string
 */
char* execute_help();

/**
 * @brief This command returns a quit message for response
 * 
 * @return char* Response string
 */
char* execute_quit();

/**
 * @brief The server will attempt to register them and respond with a string telling them that their userID has been registered successfully or not possible reasons are it was already registered or it is an invalid userID.
 * 
 * @param userID 
 * @return char* Response string
 */
char* execute_register(char* userID);

/**
 * @brief The server will respond with a string of information about the user. Requires registered user.
 * 
 * @param userID 
 * @return char* Response string
 */
char* execute_myinfo(char* userID);

/**
 * @brief The server will respond with a string of all online users. Requires registered user.
 * 
 * @param userID 
 * @return char* Response string
 */
char* execute_online_users(char* userID);

/**
 * @brief The server will respond with a string of all registered users. Requires registered user.
 * 
 * @param userID 
 * @return char* Response string
 */
char* execute_registered_users(char* userID);  

#endif
