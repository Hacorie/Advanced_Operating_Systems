#include <stdio.h>
#include <stdlib.h>

main()
{
    printf("STARTING\n");  fflush(stdout);
    while (1)
        ;
    printf("ENDING\n");  fflush(stdout);
    exit(7);
}
