/**
 * @file cipher.c
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief Handles the basic features of using a substitution cipher 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cipher.h"
#include "logging.h"

#define KEY_FILE_NOT_FOUND 1

static struct cipher substitutionCipher;


int load_cipher()
{
    log_message("SERVER", "Initializing substitution cipher...");

    // Open the key file
    FILE* keyFile = fopen(KEY_FILE, "r");
    if (!keyFile)
    {
        log_message("SERVER", "Subsitution cipher key file not found.");
        return KEY_FILE_NOT_FOUND;
    }

    // Get the cipher key as a line
    char line[500];
    fgets(line, 500, keyFile);
    close(keyFile);

    // Parse each line
    char *token;
    int tokenCount = 0, keyNum;
    token = strtok(line, " ");
    for ( ; token != NULL; tokenCount++)
    {
        keyNum = atoi(token);
        substitutionCipher.encode_list[tokenCount] = keyNum;
        substitutionCipher.decode_list[keyNum] = tokenCount;
        token = strtok(NULL, " ");
    }

    // Good load
    log_message("SERVER", "Successfully initialized cipher key.");

    return CIPHER_LOADED;
}

void encode(char* text, char* enc)
{
    for (int i = 0; i < strlen(text); i++)
        enc[i] = substitutionCipher.encode_list[text[i]-32]+32;
}

void decode(char* text, char* dec)
{
    for (int i = 0; i < strlen(text); i++)
        dec[i] = substitutionCipher.decode_list[text[i]-32]+32;
}