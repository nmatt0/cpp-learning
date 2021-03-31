#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char command[50];
    strcpy(command, "sleep 10");
    system(command);
    return 0;
}

