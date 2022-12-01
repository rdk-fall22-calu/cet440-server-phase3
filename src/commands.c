/**
 * @file commands.c
 *  @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief 
 * 
 */


#include "users.h"
#include <stdlib.h>
#include <string.h>
#include "server.h"

char* execute_help()
{
    // blue:                    \033[0;34m
    // red:                     \033[0;31m
    // reset to default color:  \033[0m
    //Quit formatting

    return "1#Commands: QUIT, HELP, REGISTER, LOGIN, MYINFO, ONLINEUSERS, REGISTEREDUSERS";

    // return  "1#"
    //         "All successful responses begin with prefix 1#\n"
    //         "All unsuccessful responses begin with prefix 0#\n"
    //         "Command: QUIT\n"
    //         "Description: Quit disconnects the client and the server.\n"
    //         "Example: 1#Disconnecting.\n"
    //         "\n"
    //         "Command: REGISTER\n"
    //         "Description: Registers a user with the server, as long as the user is part of the accepted list. If users first time registering will prompt to set password. Returns an informative response.\n"
    //         "Example: 0#Unable to register user.\n"
    //         "\n"
    //         "Command: LOGIN\n"
    //         "Description: Requires the user to be registered. Asks the user to input their password. Returns an informative response.\n"
    //         "Example: 1#Successful Login.\n"
    //         "\n"
    //         "Command: MYINFO\n"
    //         "Description: Requires the user to be logged in. MyInfo returns information about the user.\n"
    //         "Example: 1#Robert Krency#20#3.2#123.456.7.8\n"
    //         "\n"
    //         "Command: ONLINEUSERS\n"
    //         "Description: Requires the user to be logged in. Returns a list of currently active users, delimited by ‘#’.\n"
    //         "Example: 1#kre1188#chen#ste4864\n"
    //         "\n"
    //         "Command: REGISTEREDUSERS\n"
    //         "Description: Requires the user to be logged in. Returns a list of currently registered users, delimited by ‘#’.\n"
    //         "Example: 1#kre1188#chen#ste4864\n"
    //     ;
}


char* execute_quit(char* userID)
{
    // Get the user
    struct user *user = get_user(userID);

    // Check if the user exists and is logged in
    if (user != NULL && user->status == STATUS_LOGGED_IN)
        // If so, log them out.
        user->status = STATUS_REGISTERED;

    return "1#Disconnecting.";
}


char* execute_register(char* userID, int socket)
{
    // Get the user
    struct user *user = get_user(userID);

    // Check if the user exists
    if (user == NULL)
        return "0#Unable to register user.";

    // Check if the user is registered
    if (user->status == STATUS_REGISTERED)
        return "0#User is already registered.";

    //Prompt the user to set password and check if they match.
    char buff1[PWD_SIZE], buff2[PWD_SIZE];

    send_message(socket, "1#Please enter a password:");
    receive_message(socket, buff1);

    send_message(socket, "1#Please re-enter the password:");
    receive_message(socket, buff2);
    
    // Check if there is no valid password
    if (strcmp(buff1, buff2) != 0 )
    {
        return "0#Unable to register user.";
    }

    // Change user's status to registered, copy in password, and save data.
    user->status = STATUS_REGISTERED;
    strcpy(user->password, buff1);
    save_user_data();
    
    return "1#User successfully registered and password set.";

}


char* execute_login(char* userID, int socket)
{
    // Get the user
    struct user *user = get_user(userID);
    
    // Check if user exists and is registered
    if (user == NULL || user->status == STATUS_UNREGISTERED)
        return "0#User must be registered.";
        
    // Prompt the user for their password and check to see if it matches 
    send_message(socket, "Please enter your password:");
    char input[50];
    receive_message(socket, input);

    // Check if the input password is the same as the registered password.
    if ( strcmp(input, user->password) == 0 )
    {
        user->status == STATUS_LOGGED_IN;
        return "1#Successful Login.";
    } else { 
        return "0#Unsuccessful Login.";
    }
}


char* execute_myinfo(char* userID)
{
    // Get the user
    struct user *user = get_user(userID);

    // Check if the user exists and is registered
    if (user == NULL || user->status != STATUS_LOGGED_IN)
        return "0#User must be registered.";

    // Put together the info message
    char message[4000];
    snprintf(message, sizeof(message), "1#%s#%d#%1.1f#%s", user->name, user->age, user->gpa, user->address);
    return message;
}


char* execute_online_users(char* userID)
{
    // Get the user
    struct user *user = get_user(userID);

    // Check if the user exists and is registered
    if (user == NULL || user->status != STATUS_LOGGED_IN)
        return "0#User must be logged in.";

    // Put together the list of online users
    char message[4000];
    memset(message, 0, 4000);
    strcat(message, "1");
    struct user *userList = get_user_list();
    for (int i = 0; i < NUM_USERS; i++)
    {
        if (strcmp(userList[i].address, DEFAULT_IP_ADDRESS) != 0)
        {
            strcat(message, "#");
            strcat(message, userList[i].userID);
        }
    }
    return message;
}


char* execute_registered_users(char* userID)
{
    // Get the user
    struct user *user = get_user(userID);

    // Check if the user exists and is registered
    if (user == NULL || user->status != STATUS_LOGGED_IN)
        return "0#User must be logged in.";

    // Put together the list of registered users.
    char message[4000];
    memset(message, 0, 4000);
    strcat(message, "1");
    struct user *userList = get_user_list();
    for (int i = 0; i < NUM_USERS; i++)
    {
        if (userList[i].status == STATUS_LOGGED_IN)
        {
            strcat(message, "#");
            strcat(message, userList[i].userID);
        }
    }
    return message;
}
