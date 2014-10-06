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

//Maximum size of number of arguments. 
//Example: "ls | grep input.txt <" has 3 arguments.
#define ARGS_ARRAY_SIZE     20

int findRedirects(char *input);
int parse(char *inputLine, char *arguments[], const char *delimiters);
void forkIt(char *args[]);
void execToFile(char *args[], char *fileName);

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
}

int main(){
    while(1){
        printf("kash $ ");
        fgets(line, BUFFER, stdin);

        if (strcmp(line, "exit"))
            exit(1);
        else {
            int redirectStatus = findRedirects(line);
            int count = parse(line, args, DELIMS);
            forkIt(args);
        }
    }
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
    char *inputCopy = input;
    struct *redirToReturn = malloc(sizeof(struct redirCode));
    redirToReturn.argsWithoutFile[strlen(input)];
    char *temp[];

    //Do an initial search for the delimeters
    //before strtok destroys it. O(n) time.
    for (i = 0; i < strlen(input); i++){
    	int redirectOperatorReached = 0;
    	int count = 0;

        if (input[i] == '<'){
            redirToReturn.code = 1;
            redirectOperatorReached = 1;
        }
        else if (input[i] == '>'){
            redirToReturn.code = 2;
            redirectOperatorReached = 1;
        }
        if (redirectOperatorReached == 0){
        	redirToReturn.argsWithoutFile[count] = 
        	count++;
        }
    }
}

int parse(char *inputLine, char *arguments[], const char *delimiters)
{
    int count = 0;
    printf("Parsed out: ");
    for (p = strtok(inputLine, delimiters); p != NULL; p = strtok(NULL, delimiters))
    {
        arguments[count] = p;
        printf("[%s]", arguments[count]);
        count++;
    }
    putchar('\n');
    arguments[count]=NULL;
    return count;
}

void forkIt(char *args[])
{
    int pid = fork();   //Pretty much spawns a new shell.
    printf("pid of new fork: %d\n", pid);
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

void execToFile(char *args[], char *fileName){
    int out = open(fileName, O_RDWR|O_CREAT|O_APPEND, 0600);
    int save_out = dup(fileno(stdout));

    if (-1 == dup2(out, fileno(stdout)))
        perror("cannot redirect stdout");
    execvp(args[0], args);
    fflush(stdout); close(out);
    dup2(save_out, fileno(stdout));
    close(save_out);
}
