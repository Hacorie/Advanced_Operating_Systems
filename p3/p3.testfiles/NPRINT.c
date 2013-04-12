#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
    int i,n;

    printf("STARTING\n");  fflush(stdout);
    n = atoi(argv[1]);
    for (i=0; i < n; i++)
    {
        printf("pid %d\n",getpid());  fflush(stdout);
        sleep(1);
    }
    printf("ENDING\n");  fflush(stdout);
    exit(7);
}
