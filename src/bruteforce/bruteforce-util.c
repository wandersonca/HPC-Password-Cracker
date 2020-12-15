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

int assignCharInBuffer(char *passwordToTest, char *characters, int pass_idx, int buf_length, int next_idx)
{
    passwordToTest[pass_idx] = characters[next_idx % buf_length];
    next_idx = next_idx / buf_length;
    return next_idx;
}