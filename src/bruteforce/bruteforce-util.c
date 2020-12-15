#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash/hash.h"

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