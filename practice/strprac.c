#include <stdio.h>
#include <string.h>

int main()
{
    char *line;
    fgets(line, 1024, stdin);

    char *line1;
    char *line2;

    for (line1 = strtok(line, "><"); line1 != NULL; line1 = strtok(NULL, "><"))
    printf("%s\n", line1);
}
