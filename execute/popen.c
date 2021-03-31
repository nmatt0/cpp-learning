#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    FILE *fp = popen("sleep 10","r");
    char buff[50];
    while ( fgets( buff, 50, fp ) != NULL ) {
        printf("LS->%s\n", buff );
    }
    pclose(fp);
    return 0;
}



