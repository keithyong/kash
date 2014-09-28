#include <stdio.h>
#include <string.h>
#define CHAR_LENGTH         100
#define MAX_OPS             3   //Max number of operators, for example <>|< = 4 ops

const char *DELIMS = " ,.\n";
const char *commands[3];
char operators[3];
char *p;
int i;

int strlength(char *s)
{
    int n;
    for (n = 0; *s != '\0'; s++)
        n++;
    return n;
}

void operatorParse(char *inputLine, char ops[3])
{
    int operatorCount = 0;

    for (int i = 0; i < strlength(inputLine); i++){
        char c = inputLine[i];
        if ((operatorCount < MAX_OPS) && ((c == '|') || (c == '>') || (c == '<'))){
            ops[operatorCount] = c;
            operatorCount++;
            inputLine[i] = 'X';
        }
    }
}
    
int parse(char *inputLine, char *arguments[], const char *delimiters)
{
    int count = 0;
    for (p = strtok(inputLine, delimiters); p != NULL; p = strtok(NULL, delimiters))
    {
        arguments[count] = p;
        count++;
    }
    return count;
}

void printString(char *s)
{
    int n;
    for(n = 0; *s != '\0'; s++){
        printf("[%c]", s[n]);
        n++;
    }
    putchar('\n');
}   

int main()
{
    char line[1024];
    size_t bufferSize = 1024;

    char *args[CHAR_LENGTH];

    fgets(line, bufferSize, stdin);
    operatorParse(line, operators);
    int count = parse(line, args, DELIMS); 
    printString(line);
    printString(operators);
}
