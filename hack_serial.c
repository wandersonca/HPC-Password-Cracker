/*
** ALT + SHIFT + F - Formatting the code
** @Author: Ma Luo
** @Date: Nov 15, 2020
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 5
#define PASS "1acaa"

void bruteSequential(int length, char *pattern, char *pass);
char* bruteImpl(char *str, int index, int maxDepth, char *pattern, char *pass);
bool compareStr(char *a, char *b);

int main()
{
    char *pattern = "abcdefghigjlmnopqrstuvwxyz0123456789!@#$^&*";
    bruteSequential(N, pattern, PASS);
}

bool compareStr(char *a, char *b)
{
    int i = 0;
    int countLen = 0;
    for (i = 0; i < N; i++)
    {
        if ((int)a[i] == (int)b[i])
        {
            countLen += 1;
        }
        else
        {
            return false;
        }
    }
    if (countLen == N)
    {
        return true;
    }
    else
    {
        return false;
    }
}

char* bruteImpl(char *str, int index, int maxDepth, char *pattern, char *pass)
{
    int i = 0;
    for (i = 0; i < (strlen(pattern) - 1); ++i)
    {
        str[index] = pattern[i];

        if (index == maxDepth - 1)
        {
            if (compareStr(str, pass))
            {
                printf("Found pass in loop: %s\n", str);
                return str;
            }
        }
        else
            bruteImpl(str, index + 1, maxDepth, pattern, pass);
    }
}

void bruteSequential(int length, char *pattern, char *pass)
{
    char *stream = malloc(length + 1);
    int i = 0;
    for (i = 1; i <= length; ++i)
    {
        stream[i] = '\0';
        bruteImpl(stream, 0, i, pattern, pass);
    }
    free(stream);
}