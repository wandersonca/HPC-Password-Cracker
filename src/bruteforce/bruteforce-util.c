#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce.h"

/**
* @author: Willam Anderson
*
* This function is used to generate probabilities for specific password lengh and print it out when the rank encounters 0 
*
* @param number_of_characters - size of pattern set to match the hashed password
* @param length - value varies from 0 to maxium password lengh (non-hashed one)
* @param verbose - flag for showing debugging message
* @param my_rank - rank of process 0 to p
* @return possibilities - number of probabilities with password lengh - @length
*
*/

void print_stats(char *password_hash, char *characters, int number_of_characters, int password_max_length)
{
    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);
}

long calculate_possibilities(int number_of_characters, int length, int verbose, int my_rank)
{
    long possibilities = (long)pow(number_of_characters, length);
    if (verbose && my_rank == 0)
    {
        printf("Now calculating password length of %d, it has %ld possibilities\n", length, possibilities);
    }
    return possibilities;
}

/**
* @author: Willam Anderson
*
* This function is used to give actual password in the result field.
* To be notified, the operations on the result, will be effective in the device memory
*
* @param length - size of password length (non-hashed)
* @param characters - character set to compare with
* @param number_of_characters - size of characters sets
* @param seed - index to the character set, and reflecting to password set with [seed % number_of_characters] in a recursion way in the loop
* @result - buffer to hold the password to be tested with
*
*/

void generate_password(int length, char *characters, int number_of_characters, int seed, char * result)
{
    int i;
    for (i = 0; i < length; i++)
    {
        result[i] = characters[seed % number_of_characters];
        seed = seed / number_of_characters;
    }
    result[length] = '\0';
}

/**
* @author: Willam Anderson
*
* This function is used to give actual password in the result field.
* To be notified, the operations on the result, will be effective in the device memory
*
* @param counter - base counter from last step
* @param possibilities - maxium num of the probalities
* @param chunk_size - step size, each step will increase counter by @chunk_size
* @return @counter + @chunk_size - returns the counter for next step
*
*/

int calculate_next_step(int counter, int possibilities, int chunk_size)
{
    if (counter + chunk_size > possibilities)
    {
        return possibilities;
    }
    else
    {
        return counter + chunk_size;
    }    
}