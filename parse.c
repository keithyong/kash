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

    for (i = 0; i < strlength(inputLine); i++){
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
    arguments[count]=NULL;
    return count;
}

int main()
{
    char line[1024];
    size_t bufferSize = 1024;

    char *args[CHAR_LENGTH];

    fgets(line, bufferSize, stdin);
    operatorParse(line, operators);
    int count = parse(line, args, DELIMS); 
    //printf("%s", line);
    int j=0;
    while (args[j]!=NULL)
	printf("%s ",args[j++]);
    for(i = 0; i < MAX_OPS; i++)
	printf("%c", operators[i]); 
    putchar('\n');
    printf("%s", line);
}
