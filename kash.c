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

#define ARGS_SIZE   20

const char *DELIMS = " -|<>";
char *line;
size_t bufferSize = 1024;
char *output;
char *args[ARGS_SIZE];

int parse(char *inputLine, char *arguments[], const char *delimiters);

int main(){

    while(1){
        printf("kash $ ");
        getline(&line, &bufferSize, stdin);
        int count = parse(line, args, DELIMS);
        if (!strcmp(line, "exit\n")){
            exit(1);
        }
        else if (!strcmp(line, "ls\n")){
            execvp(args[0], args);
            printf("ls called\n");
        }
        else if (!strcmp(line, "cp\n")){
        }
        else{
            printf("Command not found\n");
        }
    }
}

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


