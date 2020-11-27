/* 
    remove_new_line()
    - utility function to remove new line character from each entry in the given file 
*/
int remove_new_line(char *input, char **output)
{
    int len = strlen(input);

    if(input[len-1] == '\n')
    {
        *output = (char*)malloc(sizeof(char)*(len-1));
        strncpy(*output, input, len-1);
    }
    else
    {
        *output = (char*)malloc(sizeof(char)*(len-1));
        strncpy(*output, input, len);
    }
    
    return 0;
}