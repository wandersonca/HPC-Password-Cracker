// Add any common functionality here...

/* 
    remove_new_line()
    - utility function to remove new line character from each entry in the given file 
*/
int remove_new_line(char *input, char **output)
{
    int len = strlen(input);

    if(input[len-1] == '\n')
    {
        input[len-1] = '\0';
        *output = (char*)malloc(sizeof(char)*(len));
        strncpy(*output, input, len);
    }
    else
    {
        *output = (char*)malloc(sizeof(char)*(len+1));
        sprintf(*output, "%s\0", input);
    }
    
    return 0;
}