/**
 * @file users.c
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "logging.h"


int load_users_list()
{
    log_message("SERVER", "Loading user data...");
    int status = 0;

    // Open the file
    FILE* userDataFile = fopen(USER_DATA_FILE, FILE_READ);

    // Check the file exists, if not load the accepted users and initialize data
    if (!userDataFile)
    {
        log_message("SERVER", "User data does not exist.");
        status = initialize_users();
    }
    else 
    {
        char line[FILE_BUFFER_SIZE];
        char *token;
        int userCount = 0;

        while (fgets(line, FILE_BUFFER_SIZE, userDataFile) != NULL) 
        {
            // Get the User ID
            token = strtok(line, CSV_DELIMITER);
            strcpy(userList[userCount].userID, token);
            
            // Get the User Name
            token = strtok(NULL, CSV_DELIMITER);
            strcpy(userList[userCount].name, token);

            // Get the User Age
            token = strtok(NULL, CSV_DELIMITER);
            userList[userCount].age = atoi(token);

            // Get the User GPA
            token = strtok(NULL, CSV_DELIMITER);
            userList[userCount].gpa = atof(token);

            // Get the User Status
            token = strtok(NULL, CSV_DELIMITER);
            userList[userCount].status = atoi(token);

            // Get the User IP Address
            token = strtok(NULL, CSV_DELIMITER);
            strcpy(userList[userCount].address, token);

            // Increment count
            userCount++;
        }

        // Close the file and set the status
        fclose(userDataFile);
        status = 1;
    }

    // Return the status
    if (status == 1)
        log_message("SERVER", "Successfully loaded user data.");
    else
        log_message("SERVER", "Unable to load user data.");
    return status;
}


int initialize_users()
{
    log_message("SERVER", "Initializating user data...");
    int status = 0;

    // Open the file
    FILE* acceptedUsersFile = fopen(ACCEPTED_USERS_FILE, FILE_READ);

    // Check the file exists, if not load the accepted users and initialize data
    if (!acceptedUsersFile)
    {
        log_message("SERVER", "Unable to load accepted users file.");
        status = 0;
    }
    else
    {
        char line[FILE_BUFFER_SIZE];
        char *token;
        srand(time(0));
        int userCount = 0;

        // Loop through all data in the file
        while (fgets(line, FILE_BUFFER_SIZE, acceptedUsersFile) != NULL)
        {
            // Get the User ID
            token = strtok(line, " \t");
            strcpy(userList[userCount].userID, token);

            // Get the User Name
            token = strtok(NULL, "\n");
            strcpy(userList[userCount].name, token);

            // Initialize Values
            userList[userCount].age = (rand() % 5) + 18; // Value between 18-22
            userList[userCount].gpa = ((rand() % 16) + 25) / 10.0f; // Value between 2.5-4.0
            userList[userCount].status = UNREGISTERED;
            strcpy(userList[userCount].address,DEFAULT_IP_ADDRESS);

            // Increment count
            userCount++;
        }

        // Close the file and set the status
        fclose(acceptedUsersFile);
        save_user_data();
        status = 1;
    }
    
    // Return the status of the operation;
    if (status == 1)
        log_message("SERVER", "Successfully initialized user data.");
    else
        log_message("SERVER", "Unable to initialize user data.");
    return status;
}


void save_user_data()
{
    log_message("SERVER", "Saving user data...");
    // Open the file
    FILE* userDataFile = fopen(USER_DATA_FILE, FILE_WRITE);

    // For each student, write out the data
    int i;
    for (i = 0; i < NUM_USERS; i++)
    {
        fprintf(userDataFile, "%s,%s,%d,%1.1f,%d,%s,\n",
            userList[i].userID,
            userList[i].name,
            userList[i].age,
            userList[i].gpa,
            userList[i].status,
            userList[i].address);
    }

    // Close the file
    fclose(userDataFile);

    // Return
    log_message("SERVER", "Saved user data.");
}


struct user* get_user(char *userID)
{

    for (int i = 0; i < NUM_USERS; i++)
    {
        if ( strcmp(userID, userList[i].userID) == 0 )
            return &userList[i];
    }

    // Cannot find user
    return NULL;

}

struct user* get_user_list()
{
    return userList;
}
