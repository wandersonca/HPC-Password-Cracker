/*
** ALT + SHIFT + F - Formatting the code
** @Author: Ma Luo
** @Date: Nov 15, 2020
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 3
#define PASS "1ac"

void bruteSequential(int length, char *pattern, char *pass);
void bruteImpl(char *str, int index, int maxDepth, char *pattern, char *pass);
bool compareStr(char *a, char *b);

int main()
{
    char *pattern = "abcdefghigjlmnopqrstuvwxyz0123456789!@#$^&*";
    bruteSequential(N, pattern, PASS);
    //printf("%d",compareStr("1a","1a"));

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

void bruteImpl(char *str, int index, int maxDepth, char *pattern, char *pass)
{
    for (int i = 0; i < (strlen(pattern) - 1); ++i)
    {
        str[index] = pattern[i];

        if (index == maxDepth - 1)
        {
            if (compareStr(str, pass))
            {
                printf("Found pass in loop: %s\n", str);
                break;
            }
        }
        else
            bruteImpl(str, index + 1, maxDepth, pattern, pass);
    }
}

void bruteSequential(int length, char *pattern, char *pass)
{
    char *stream = malloc(N + 1);
    for (int i = 1; i <= N; ++i)
    {
        stream[i] = '\0';
        bruteImpl(stream, 0, i, pattern, pass);
    }
    free(stream);
}