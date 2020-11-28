#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void bruteSequential(int length, char *pattern, char *pass);                   //Entry point of a loop
char* bruteImpl(char *str, int index, int maxDepth, char *pattern, char *pass); // Recursive function
bool compareStr(char *a, char *b);                                             // safe comparing the characters one by one
int bruteforce_crack(char *password_hash, char *characters, int length, int verbose);

int bruteforce_crack(char *password_hash, char *characters, int length, int verbose)
{
    printf("Brute force of hash: %s\n", password_hash);
    printf("Using %d of characters: %s\n", length, characters);

    static unsigned char buffer[65];
    // we should get to this by brute force...
    char passwordToTest[] = "test";//Original password
    if (verbose)
    {
        printf("Hash to compare: %s\n", password_hash);
    }
    
    hash(passwordToTest, buffer);//Hash value of the password
    int bufLen = strlen(buffer);

    for (int i = 0; i < 1; i++)
    {
        if (verbose)
        {
            printf("`%s` -> %s\n", passwordToTest, buffer);
        }

        bruteSequential(bufLen,characters,buffer); //print function is inside the recursion
    }

    return 1;
}

bool compareStr(char *a, char *b)
{
    int i = 0;
    int countLen = 0;
    int len = strlen(a);
    for (i = 0; i < len; i++)
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
    if (countLen == len)
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
                printf("Password found: %s\n", str);
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