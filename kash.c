//DUE OCTOBER 1st!
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

//How many commands this shell has.
//Only 1 for now unless more custom
//commands are added.
#define COM_LENGTH          1
char *com[COM_LENGTH] = {"exit"};

//Used to split up input around <>| delims
void preParse(char *inputLine);

int parse(char *inputLine, char *arguments[], const char *delimiters);
int findCommand(char *input, char *listOfCommands[], int n);
void noCommand(char *args[]);
void runCommand(int command);

const char *DELIMS = " |<>\n";

#define BUFFER              1024
char line[BUFFER];
char *args[ARGS_ARRAY_SIZE];

//Some inits for the for loops.
int i;
char *p;

int main(){
    while(1){
        printf("kash $ ");
        fgets(line, BUFFER, stdin);
        preParse(line);
        int count = parse(line, args, DELIMS);
        int command = findCommand(args[0], com, COM_LENGTH);
        runCommand(command); 
    }
}

/*  preParse:
 *  Looks for ">", "<", or "|" and takes appropriate action
 */
void preParse(char *inputLine){
    char *after[3];
    if (strstr(inputLine, ">") != NULL){
        after[0] = strstr(inputLine, ">") + 1;
        
    }

    if (strstr(inputLine, "<") != NULL){
        after[1] = strstr(inputLine, "<") + 1;
    }

    if (strstr(inputLine, "|") != NULL){
        after[2] = strstr(inputLine, "|") + 1;
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


int findCommand(char *input, char *listOfCommands[], int n){
    for (i = 0; i < n; i++){
        if(strcmp(input, listOfCommands[i]) == 0){
          return i;
        }
    }

    //If no commands are found then return -1
    return -1;
}

void runCommand(int command){
    switch (command){
        case -1:
            noCommand(args);
            break;
        case 0:
            exit(1);
            break;
    }
}

void noCommand(char *args[])
{
    int pid = fork();
    if (pid == 0){
        execvp(args[0],args);
        printf("Command not found\n");
    }
    else{
        int status;
        waitpid(pid, &status, WCONTINUED);
    }
}
