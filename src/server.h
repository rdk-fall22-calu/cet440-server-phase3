/**
 * @file socket.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/

#ifndef _SERVER_H
#define _SERVER_H


#define RCV_BUFFER_SIZE 1024
#define SND_BUFFER_SIZE 1024
#define PORT 4401

/**
 * @brief Contains the IP Address and Socket for the specific client
 * 
 */
struct clientInfo {
    char ipaddress[15];
    void *socket;
};

/**
 * @brief Set up server connection
 * 
 */
int setup_server();


/**
 * @brief Starts the server
 * 
 * @param connection_handler The function to call for connection handling
 */
int start_server(void * (*connection_handler)(void *));


/**
 * @brief Encrypts the message to be sent, and sends it to the specified socket
 * 
 * @param socket 
 * @param message 
 * 
 */
void send_message(int socket, char *message);


/**
 * @brief Receive a message from the client
 * 
 * @param socket 
 * @param message 
 * @return int Size of the message received
 */
int receive_message(int socket, char *message);



#endif