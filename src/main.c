/**
 * @file main.c
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "logging.h"
#include "users.h"
#include "commands.h"
#include "server.h"
#include "cipher.h"

#define RCV_BUFFER_SIZE 1024
#define SND_BUFFER_SIZE 1024

int clientNumber = 0; // Store how many clients are connected

/**
 * @brief Threading function
 * 
 * @return void* 
 */
void *connection_handler(void *);

/**
 * @brief Convert a string to all upper case
 * 
 * @param text 
 * @return char* 
 */
char *strupr(char * text);

/**
 * @brief Entry point and main loop for the server
 * 
 * @return int Exit message
 */
int main()
{
    initialize_logging();
    log_message("SERVER", "Starting server.");
    
    // Load the subsitution cipher
    if ( load_cipher() == KEY_FILE_NOT_FOUND)
        return 1;

    // Load the accepted users list from file
    if(!load_users_list())
    {
        log_message("SERVER", "Unable to load accepted users list, exiting.");
        return 1;
    }

    // Initialize the server
    if (setup_server() > 0)
    {
        log_message("SERVER", "Unable to setup server, exiting.");
        return 1;
    }
     
    // Start the server
    if ( start_server(connection_handler) > 0 )
    {
        log_message("SERVER", "Server error.");
    }
     
    log_message("SERVER", "Exiting...");
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *clientInfo)
{
    //Get the socket descriptor
    struct clientInfo *info = clientInfo;
    int sock = *(int*) info->socket;
    int read_size;
    char *message , mymessage[4000],  client_message[4000], threadName[20],
         userID[10], displayMessage[4000];
    snprintf(threadName, sizeof(threadName), "SERVER\\%d", ++clientNumber);
     
    // Greet the Client
    snprintf(displayMessage, sizeof(displayMessage), "Connecting to client at %s.", info->ipaddress);
    log_message(threadName, displayMessage);

    snprintf(mymessage, sizeof(mymessage), "Greetings! You are the No.%d client. I am your connection handler. Please input your User ID.", clientNumber);
    message = mymessage;
    send_message(sock , message);

    // TODO: Get the user ID after first connection
    read_size = receive_message(sock , client_message);
    if (read_size <= 0)
    {
        log_message(threadName, "Error receiving User ID.");
        return 0;
    }
    
    // Set the user ID to lower case
    strcpy(userID, client_message);
    for (int i = 0; userID[i]; i++)
        userID[i] = tolower(userID[i]);
    
    // Return a response with the user ID
    snprintf(displayMessage, sizeof(displayMessage), "Connected with user: %s", userID);
    log_message(threadName, displayMessage);
    send_message(sock , displayMessage);
    
    // Add the user ID to the thread name for logging
    snprintf(threadName, sizeof(threadName), "SERVER\\%d %s", clientNumber, userID);

    // Set the user's IP Address
    struct user *u = get_user(userID);
    if (u != NULL)
    {
        strcpy(u->address, info->ipaddress);
        save_user_data();
    }
     
    //Receive a message from client
    int quit = 0;
    while( (read_size = receive_message(sock, client_message)) > 0 )
    {
        // End of string markers
        strupr(client_message);

        // Determine the command
        if (strcmp(client_message, CMD_HELP) == 0)
        {
            log_message(threadName, "Executing HELP command.");
            message = execute_help();
        }
        else if (strcmp(client_message, CMD_QUIT) == 0)
        {
            log_message(threadName, "Executing QUIT command.");
            message = execute_quit();
            quit = 1;
        }
        else if (strcmp(client_message, CMD_REGISTER) == 0)
        {
            log_message(threadName, "Executing REGISTER command.");
            message = execute_register(userID, sock);
        }
        else if (strcmp(client_message, CMD_LOGIN) == 0)
        {
            log_message(threadName, "Executing LOGIN command.");
            message = execute_login(userID, sock);
        }
        else if (strcmp(client_message, CMD_MYINFO) == 0)
        {
            log_message(threadName, "Executing MYINFO command.");
            message = execute_myinfo(userID);
        }
        else if (strcmp(client_message, CMD_ONLINE_USERS) == 0)
        {
            log_message(threadName, "Executing ONLINEUSERS command.");
            message = execute_online_users(userID);
        }
        else if (strcmp(client_message, CMD_REGISTERED_USERS) == 0)
        {
            log_message(threadName, "Executing REGISTEREDUSERS command.");
            message = execute_registered_users(userID);
        }
        else 
        {
            snprintf(displayMessage, sizeof(displayMessage), "Command not recognized. Client message: %s", client_message);
            log_message(threadName, displayMessage);
            message = "0#Command not recognized.";
        }
		
		//Send the message back to client
        send_message(sock , message);
		
		//clear the message buffer
		memset(client_message, 0, 4000);

        // Disconnect
        if (quit == 1)
            break;
    }

    // Set the user's state to not logged in if they are logged in
    if (u != NULL && u->status == STATUS_LOGGED_IN)
    {
        u->status = STATUS_REGISTERED;
        save_user_data();
    }
     
    if(read_size == 0)
    {
        log_message(threadName, "Client disconnected.");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        log_message(threadName, "Receive failed.");
    }
         
    return 0;
} 

char *strupr(char * text)
{
	int i, j=strlen(text);
	
	for (i=0;i<j;i++)
	{
		if ((text[i]>=97)&&(text[i]<=122))
			text[i]=text[i]-32;
	}
	return text;
}
