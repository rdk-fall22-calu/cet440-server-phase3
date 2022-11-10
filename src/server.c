/*
 *@file server.c
 *@author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
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

#define RCV_BUFFER_SIZE 1024
#define SND_BUFFER_SIZE 1024
#define PORT 4400

int clientNumber = 0; // Store how many clients are connected

struct clientInfo {
    char ipaddress[15];
    void *sock;
};

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
    log_message("SERVER", "Starting server.");

    // Load the accepted users list from file
    if(!load_users_list())
    {
        log_message("SERVER", "Unable to load accepted users list, exiting.");
        return 1;
    }

    // Initialize the server
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    // Create the Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) // Socket failed, exit
    {
        log_message("SERVER", "Socket creation failed, exiting.");
        return 1;
    }
    log_message("SERVER", "Socket created.");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 4400 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        log_message("SERVER", "Bind failed.");
        return 1;
    }
    log_message("SERVER", "Bind successful.");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept any incoming connection
    log_message("SERVER", "Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        log_message("SERVER", "Connection accepted.");
        	clientNumber ++; //increase the number of connected clients

        struct clientInfo info;
        info.sock = &client_sock;
        strcpy(info.ipaddress, inet_ntoa(client.sin_addr));
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &info) < 0)
        {
            log_message("SERVER", "Could not create thread. Exiting...");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        log_message("SERVER", "Handler assigned.");
    }
     
    if (client_sock < 0)
    {
        log_message("SERVER", "Accept failed. Exiting...");
        return 1;
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
    int sock = *(int*) info->sock;
    int read_size;
    char *message , mymessage[4000],  client_message[4000], threadName[20],
         userID[10], displayMessage[4000];
    snprintf(threadName, sizeof(threadName), "SERVER\\%d", clientNumber);
    
     
    // Greet the Client
    snprintf(displayMessage, sizeof(displayMessage), "Connecting to client at %s.", info->ipaddress);
    log_message(threadName, displayMessage);

    snprintf(mymessage, sizeof(mymessage), "Greetings! You are the No.%d client. I am your connection handler.\nPlease input your User ID.\n", clientNumber);
    message = mymessage;
    write(sock , message , strlen(message));

    // TODO: Get the user ID after first connection
    read_size = recv(sock , client_message , 4000 , 0);
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
    write(sock , displayMessage , strlen(displayMessage));

    // Set the user's IP Address
    struct user *u = get_user(userID);
    if (u != NULL)
    {
        strcpy(u->address, info->ipaddress);
        save_user_data();
    }
     
    //Receive a message from client
    int quit = 0;
    while( (read_size = recv(sock , client_message , 4000 , 0)) > 0 )
    {
        // End of string marker
		client_message[read_size] = '\0';
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
            message = execute_register(userID);
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
            snprintf(displayMessage, sizeof(displayMessage), "Command not recognized. Client message: \n\n\t %s \n\n", client_message);
            log_message(threadName, displayMessage);
            message = "0#Command not recognized.";
        }
		
		//Send the message back to client
        write(sock , message , strlen(message));
		
		//clear the message buffer
		memset(client_message, 0, 4000);

        // Disconnect
        if (quit == 1)
            break;
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

    // Clear the user's IP Address
    if (u != NULL)
    {
        strcpy(get_user(userID)->address, DEFAULT_IP_ADDRESS);
        save_user_data();
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
