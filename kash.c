#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h> 
#include <signal.h>

char line[255];

int main(){
    while(1){
        printf("ka$h >");
        getline(line);
        printf(line);
    }
}
