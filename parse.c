#include <stdio.h>
#include <string.h>
#define CHAR_LENGTH         100

const char *DELIMS = " -|<>";
 
int parse(char *inputLine, char *arguments[], const char *delimiters)
{
    int count = 0;
    for (char *p = strtok(inputLine, delimiters); p != NULL; p = strtok(NULL, delimiters))
    {
        arguments[count] = p;
        count++;
    }
    return count;
}

int main()
{
    char *line;
    size_t bufferSize = 1024;

    char *args[CHAR_LENGTH];

    getline(&line, &bufferSize, stdin);
    int count = parse(line, args, DELIMS); 
    for (int i = 0; i <= count; i++){ 
        printf("[%s]", args[i]); 
    } 
}
