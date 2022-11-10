/**
 * @file users.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief Defines the registered users 
 * 
 */

#ifndef __USERS_H__
#define __USERS_H__

#define NUM_USERS 27
#define REGISTERED 1
#define UNREGISTERED 0

#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_BUFFER_SIZE 256
#define ACCEPTED_USERS_FILE "names.txt"
#define USER_DATA_FILE "users.csv"
#define CSV_DELIMITER ","

#define DEFAULT_IP_ADDRESS "-1.-1.-1.-1"


/**
 * @brief Defines a User
 * 
 */
struct user {
    char userID[10];
    char name[50];
    int age;
    float gpa;
    char address[50]; // IP Address user is connected from
    int status;
};

static struct user userList[NUM_USERS];

/**
 * @brief Loads the list of accepted users from the file names.txt
 * 
 * @return int 1 for successful loading, 0 for failed to load
 */
int load_users_list();


/**
 * @brief Initializes random values for each user 
 */
int initialize_users();


/**
 * @brief Saves the user data to a CSV file.
 * 
 */
void save_user_data();


/**
 * @brief Returns the user with the specified user ID
 * 
 * @param userID
 * @return Pointer to user struct, NULL if user does not exist
 */
struct user* get_user(char *userID);


/**
 * @brief Gets the list of users 
 * 
 * @return user* pointer to the user list
 */
struct user* get_user_list();

#endif
