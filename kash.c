#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h> 
#include <signal.h>
#include <dirent.h>
#include <ctype.h>

//Maximum size of number of arguments. 
//Example: "ls | grep input.txt <" has 3 arguments.
#define ARGS_ARRAY_SIZE     20

struct redirCode findRedirects(char *input);
int parse(char *inputLine, char *arguments[], const char *delimiters);
void forkIt(char *args[]);
void execToFile(char *args[], char *fileName);
void execFromFile(char *args[], char *fileName);
char * removeSpaces(char * source, char * target);

const char *DELIMS = " |<>\n";

#define BUFFER              1024
char line[BUFFER];
char *args[ARGS_ARRAY_SIZE];

//Some inits for the for loops.
int i;
char *p;

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
    char *temp = redirToReturn->fileName;
    removeSpaces(redirToReturn->fileName, temp);
    redirToReturn->fileName = temp;
    return *redirToReturn;
}

int parse(char *inputLine, char *arguments[], const char *delimiters)
{
    int count = 0;
    for (p = strtok(inputLine, delimiters); p != NULL; p = strtok(NULL, delimiters))
    {
        arguments[count] = p;
        count++;
    }
    putchar('\n');
    arguments[count]=NULL;
    return count;
}


void forkIt(char *args[])
{
    int pid = fork();   //Pretty much spawns a new shell.
    if (pid == 0){
        execvp(args[0], args);
        printf("Command not found\n");
        exit(1);        //Exits out of duplicate shell.
    }
    else{
        int status;
        waitpid(pid, &status, WCONTINUED);
    }
}

void execToFile(char *args[], char *fileName)
{
    // http://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file
    int pid = fork();
    if (pid == 0){
        int fd = open(fileName, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
        if (-1 == dup2(fd, 1))
            perror("cannot redirect stdout\n");
        dup2(fd, 1);
        close(fd);
        execvp(args[0], args);
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, WCONTINUED);
    }
}

void execFromFile(char *args[], char *fileName){
    void *buffer;
    int BUF_SIZE = 1024;
    int pid = fork();
    if (pid == 0){
        int fd = open(fileName, O_RDONLY);

        int read_size = 0;
        while ((read_size = read(fd, buffer, BUF_SIZE)) != 0) {
            write(STDIN_FILENO, buffer, read_size);
        }
        close(fd);
        execvp(args[0], args);
    } else {
        int status;
        waitpid(pid, &status, WCONTINUED);
    }
}

//FROM: http://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
char * removeSpaces(char * source, char * target)
{
     while(*source++ && *target)
     {
        if (!isspace(*source)) 
             *target++ = *source;
     }
     return target;
}

int main(){
    while(1){
        printf("kash $ ");
        fgets(line, BUFFER, stdin);
        if (strcmp(line, "exit\n") == 0){
            exit(1);
        }
        else if (strcmp(line, "resume\n") == 0){
            //TODO: Resume functionality
        } else {
            struct redirCode tempRedir = findRedirects(line);
            int count = parse(tempRedir.argsWithoutFile, args, DELIMS);
            if (tempRedir.code == 2){
                //Redirect output to stdout
                execToFile(args, tempRedir.fileName);
            } 
            else if (tempRedir.code == 1){
                //Redirect file to stdin
                execFromFile(args, tempRedir.fileName);
            } else { 
                int count = parse(line, args, DELIMS);
                forkIt(args);
            }
        }
    }
}
