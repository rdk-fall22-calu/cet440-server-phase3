/**
 * @file server.c
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/


#ifndef __SERVER_C__
#define __SERVER_C__

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "server.h"
#include "logging.h"
#include "cipher.h"


static int clientNumber = 0;
static int socket_desc, client_sock, c;

int setup_server()
{
    log_message("SERVER", "Setting up server.");

    // Initialize the server
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
    server.sin_port = htons( PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        log_message("SERVER", "Socket bind failed.");
        return 1;
    }
    log_message("SERVER", "Socket bind successful.");

    // Success
    log_message("SERVER", "Server setup successful.");
    return 0;
}


int start_server( void (*connection_handler)(void *) )
{
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
     
    log_message("SERVER", "Shutting down server.");
    return 0;
}


void send_message(int socket, char *message)
{
    // Encode the message
    encode(message);

    // Send the message
    write( socket, message, strlen(message));
}


int receive_message(int socket, char *client_message)
{
    // Receive the message
    int read_size;
    read_size = recv(socket, client_message, RCV_BUFFER_SIZE, 0);

    // Check message size to ensure we received a message
    if (read_size > 0)
    {
        // If so, decode the message
        decode(client_message);
    }

    // Return the size of the message received
    return read_size;
}


#endif