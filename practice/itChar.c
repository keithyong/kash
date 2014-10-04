#include <stdio.h>

int main(){
    const char *const hellos = "hello world";
    const char *hello = hellos;
    while(hello != NULL && *hello != '\0')
    {
        printf("%s", hello);
        hello++;
    }
}
