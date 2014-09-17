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

char *line;
size_t bufferSize = 1024;

char *output;

int main(){
    while(1){
        printf("kash> ");
        getline(&line, &bufferSize, stdin);
        if (!strcmp(line, "exit\n")){
            exit(1);
        }
        else if (!strcmp(line, "ls\n")){
            opendir(output);
            printf("%s", output);
        }
        printf("%s", line);
    }
}
