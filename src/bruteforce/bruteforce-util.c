#include <math.h>

/**
* This function prints the stats for the calculation
*
* @param password_hash - the password hash to crack
* @param characters - character set to compare with
* @param number_of_characters - size of character sets
* @param password_max_length - max length of password to test
*/
void print_stats(char *password_hash, char *characters, int number_of_characters, int password_max_length)
{
    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);
}

/**
* This function is used to generate probabilities for specific password lengh and print it out when the rank encounters 0 
*
* @param number_of_characters - size of pattern set to match the hashed password
* @param length - value varies from 0 to maxium password lengh (non-hashed one)
* @param verbose - flag for showing debugging message
* @param my_rank - rank of process 0 to p
* @return possibilities - number of probabilities with password lengh - @length
*
*/
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
* This function generates a unique password for the brute force implementation.
*
* @param length - size of password length (non-hashed)
* @param characters - character set to compare with
* @param number_of_characters - size of characters sets
* @param seed - seed value to generate a unique password for testing
* @result - buffer to hold the password to be tested with
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
* Generates the next loop step size for the brute force implentation.
*
* @param counter - base counter from last step
* @param possibilities - maximum num of the probalities
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