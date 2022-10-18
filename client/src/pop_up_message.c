#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define YELLOW "\x1B[33m"
#define NORMAL "\x1B[0m"
#define RED "\x1B[91m"
#define GREEN "\x1B[32m"

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[])
{
    if(argc < 3){return FALSE;}

    printf("%s", argv[1]);

	printf("\n\n    ");
    for(int i = 2; i < argc; ++i)
	    printf("%s ", argv[i]);
	printf("\n");

    sleep(2);
    
    return TRUE;
}