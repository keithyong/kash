#include <stdio.h>
#include <stdlib.h>

int main(int arg, char *args[]){
    FILE *fp;
    printf("%s", args[1]);
    if ((fp = fopen(args[1], "r")) == NULL)
        printf("NULL file\n");
    else
        printf("file opened!");

    char c;
    while((c = fgetc(fp)) != EOF)
            putchar(c);


    fclose(fp);
}
