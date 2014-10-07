#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFFER      1024
struct redirCode findRedirects(char *input);
struct redirCode {
    /* For code:
     * 0 = none
     * 1 = stdin
     * 2 = stdout
     */
    int code;
    char *argsWithoutFile;
    char *fileName;
};

int main(){
    const char *delims = "<>";
    struct redirCode temp;
    char line[BUFFER];

    printf("Input: ");
    fgets(line, 1024, stdin);
    temp = findRedirects(line);

    printf("temp.code = %d\n", temp.code);
    printf("temp.fileName = %s\n", temp.fileName);
    printf("temp.argsWithoutFile = %s\n", temp.argsWithoutFile);

}

/* Looks for '>', '<' in a string.
 * Will destroy string *input
 * Returns a redirCode struct with:
 * 1. fileName - the name of file that
 * wants to be redirected/stdin
 * 2. code - the direction of redirect
 * 3. args - the arguments w/o filename
 * */
struct redirCode findRedirects(char *input)
{
    const char *delims = "<>";
    struct redirCode *redirToReturn = malloc(sizeof(struct redirCode));

    //Do an initial search for the delimeters
    //before strtok destroys it. O(n) time.
    int redirectOperatorReached = 0;
    int count = 0;

    int i;
    for (i = 0; input[i] != 0; i++){
        if (input[i] == '<'){
            redirToReturn->code = 1;
            redirectOperatorReached = 1;
            input[i] = ' ';
        }
        else if (input[i] == '>'){
            redirToReturn->code = 2;
            redirectOperatorReached = 1;
            input[i] = ' ';
        }
        if (redirectOperatorReached != 1){
            count++;
        }
    }
    int lengthOfInput = strlen(input);

    int sizeOfMalloc = (lengthOfInput+1)*sizeof(char);
    redirToReturn->argsWithoutFile = (char *) malloc(sizeOfMalloc);
    redirToReturn->fileName = (char *) malloc(sizeOfMalloc);

    strncpy(redirToReturn->argsWithoutFile, input, count);
    redirToReturn->argsWithoutFile[count] = '\0';

    strncpy(redirToReturn->fileName, input + count, lengthOfInput - count);

    return *redirToReturn;
}

