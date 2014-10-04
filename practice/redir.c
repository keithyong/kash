#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int out = open("cout.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    int save_out = dup(fileno(stdout));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }

    char *args[] = {"-a"};
    execvp("ls", args);
    puts("hello world");

    fflush(stdout); close(out);

    dup2(save_out, fileno(stdout));

    close(save_out);

    puts("back to normal output");

    return 0;
}
