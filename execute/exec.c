#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    char *args[]={"/bin/ls","-la"};
    execvp(args[0],args);
    return 0;
}

