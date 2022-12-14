/**
 * @file cipher.h
 * @author Distint Howie (how4685@pennwest.edu)
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Anthony Stepich (ste4864@pennwest.edu)
 * @brief Handles the basic features of using a substitution cipher 
 * 
 */

#ifndef _CIPHER_H
#define _CIPHER_H

#define KEY_FILE "key.txt"
#define KEY_FILE_NOT_FOUND 1
#define CIPHER_LOADED 0


struct cipher {
    char encode_list[95];
    char decode_list[95];
};

/**
 * @brief Load the cipher key from memory and initialize the cipher
 * 
 */
int load_cipher();


/**
 * @brief Encode the text into enc using the substitution cipher
 * 
 * @param text 
 */
void encode(char* text, char* enc);


/**
 * @brief Decode the text into dec using the substitution cipher
 * 
 * @param text 
 */
void decode(char* text, char* dec);


#endif