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

// http://stackoverflow.com/a/4024086/4117152
// This stack is used to keep track of PIDs 
// in the heap for sigint and sigtstp handling
int stack[50];
int *pidstack;
int stackCount;
#define push(pidstack, n) (*((pidstack)++) = (n))
#define pop(pidstack) (*--(pidstack))

struct redirCode findRedirects(char *input);
int parse(char *inputLine, char *arguments[], const char *delimiters);
void forkIt(char *args[], int background);
void execToFile(char *args[], char *fileName, int background);
void execFromFile(char *args[], char *fileName, int background);
char * removeSpaces(char * source, char * target);
int myFork();

const char *DELIMS = " |<>\n";

#define BUFFER              1024
char line[BUFFER];
char *args[ARGS_ARRAY_SIZE];

int global_PID;

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
    arguments[count]=NULL;
    return count;
}


void myExecvp(const char *p, char  *const argv[]){
    execvp(args[0], args);
    printf("Command not found\n");
    pop(pidstack);
    stackCount--;
    exit(1);
}

void forkIt(char *args[], int background)
{
    int pid = fork();
    if (pid == 0){
        myExecvp(args[0], args);
    }
    if (background == 0){
        int status;
        waitpid(pid, &status, WCONTINUED);
    } else {
        printf("Process created in background, pid = %d", pid);
    }
}

void execToFile(char *args[], char *fileName, int background)
{
    // http://stackoverflow.com/questions/2605130/redirecting-exec-output-to-a-buffer-or-file
    int pid = fork();
    if (pid == 0){
        int fd = open(fileName, O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
        if (-1 == dup2(fd, 1))
            perror("cannot redirect stdout\n");
        dup2(fd, 1);
        close(fd);
        myExecvp(args[0], args);
    }
    if (background == 0){
        int status;
        waitpid(pid, &status, WCONTINUED);
    } else {
        printf("Process created in background, pid = %d", pid);
    }
}

void execFromFile(char *args[], char *fileName, int background){
    void *buffer;
    int BUF_SIZE = 1024;
    int pid = fork();
    if (pid == 0){
        int fd = open(fileName, O_RDONLY);
        dup2(fd, 0);
        close(fd);
        myExecvp(args[0], args);
    } 
    if (background == 0){
        int status;
        waitpid(pid, &status, WCONTINUED);
    } else {
        printf("Process created in background, pid = %d", pid);
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

void sigint_handler(int signo)
{
    printf("received SIGINT\n");
}
void sigtstp_handler(int signo)
{
    printf("received SIGTSTP\n");
}

int parseForBackground(char *line)
{
    char *temp;
    char *tempArgs[ARGS_ARRAY_SIZE];
    temp = line;
    int count = parse(temp, tempArgs, DELIMS);
    return (strcmp(tempArgs[count - 1], "&") == 0);
}

int main(){
    pidstack = stack; //Init the global PID stack
    int isBackgroundProcess = 0;
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    int background = 0;
    while(1){
        printf("kash $ ");
        fgets(line, BUFFER, stdin);
        if (strcmp(line, "exit\n") == 0){
            exit(1);
        }
        else if (strcmp(line, "resume\n") == 0){
            //TODO: Resume functionality
        }
        else if (parseForBackground(line)){
            printf("background job detected\n");
            background = 1;
        }
        else {
            struct redirCode tempRedir = findRedirects(line);
            int count = parse(tempRedir.argsWithoutFile, args, DELIMS);
            if (tempRedir.code == 2){
                //Redirect output to stdout
                execToFile(args, tempRedir.fileName, background);
            } 
            else if (tempRedir.code == 1){
                //Redirect file to stdin
                execFromFile(args, tempRedir.fileName, background);
            } else { 
                int count = parse(line, args, DELIMS);
                forkIt(args, background);
            }
        }
    }
}
