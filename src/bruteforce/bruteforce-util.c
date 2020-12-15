#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"
#include "bruteforce.h"

/*
* @Author: Ma Luo
* 
* @passwordToTest - password to test with, values edits would be effective in the memory
* @characters - character set to compare with
* @pass_idx - password index
* @buf_length - length of the character set, since we have it in the original call, no need to add more space complexity here
* @next_idx - help decide next val to assign into passwordToTest
*
*/

int assignCharInBuffer(char *passwordToTest, char *characters, int pass_idx, int buf_length, int next_idx);

/*
* @Author: Ma Luo
* 
* @passhash - hashed password with sha256 values
* @buf - character set to compare with
* @pass - actual password we test with
* @return result - 1 (not found)/ 0 (found)
*/

int findPasswordOrNo(char *passhash, char *buf, char *pass);

/*
* @Author: Ma Luo
* 
* @base - starting point of a number set (smallest)
* @max - maxium value of a set of numbers
* @step - within each step, we are going to increase base by one step untill it reach the maxium value
* @return nextStep - return base + step or keep the maxium value as the ending point
*/

int climbToMax(int base, int max, int step);

/*
* @Author: Ma Luo
* 
* @pass - matched password if we have found it
* @check - either 1 / 0, please look at the micro value for PASS_FOUND & PASS_NOT_FOUND
* 
* PASS_FOUND 0
* PASS_NOT_FOUND 1
*
*/

void printPassIfFound(char *pass, int check);

void Greetings(char *password_hash, int password_max_length, char *characters, int number_of_characters);

int assignCharInBuffer(char *passwordToTest, char *characters, int pass_idx, int buf_length, int next_idx)
{
    passwordToTest[pass_idx] = characters[next_idx % buf_length];
    next_idx = next_idx / buf_length;
    return next_idx;
}

int findPasswordOrNo(char *passhash, char *buf, char *pass)
{
    int result = 1;
    if (!strcmp(passhash, buf))
    {
        printf("Password found: %s\n", pass);
        result = 0;
    }
    return result;
}

int climbToMax(int base, int max, int step)
{
    int nextStep;
    if (base + step > max)
    {
        nextStep = max;
    }
    else
    {
        nextStep = base + step;
    }
    return nextStep;
}

void printPassIfFound(char *pass, int check)
{
    if (check == PASS_FOUND)
    {
        printf("Password found: %s\n", pass);
    }
    else if (check == PASS_NOT_FOUND)
    {
        printf("Password not found.\n");
    }
}

void Greetings(char *password_hash, int password_max_length, char *characters, int number_of_characters)
{
    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d characters: %s\n", number_of_characters, characters);
    printf("Calculating to a length of %d\n", password_max_length);
}

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