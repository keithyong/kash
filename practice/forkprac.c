#include <unistd.h>
#include <stdio.h>

pid_t fork(void);

int main()
{
    pid_t childProcess;

    int life = 42;

    switch(childProcess = fork())
    {
        case -1:
            printf("ERROR");
            break;
        case 0:
            life = 69;
            printf("calling PID = %d\n", getpid());
            printf("forkPID = %d\n", childProcess);
            printf("%d\n", life);
            break;
        default:
            sleep(2);
            printf("%d\n", life);
            break;
    }
}
