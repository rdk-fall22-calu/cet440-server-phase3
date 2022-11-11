/**
 * @file socket.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
*/

#ifndef __SERVER_H__
#define __SERVER_H__


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
 */
void start_server();


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