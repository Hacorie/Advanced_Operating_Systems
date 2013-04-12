#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/resource.h>
#include <sys/wait.h>

main()
{
    int i;
    char *p;
    struct rlimit rl;

    rl.rlim_cur = 2000000;    /* try also at 4000000 */
    rl.rlim_max = 2000000;

    // i = setrlimit(RLIMIT_AS,&rl);    /* instead of RLIMIT_DATA */
    for (i=0; ;i++)
    {
        p = malloc(2000);
        if (p == NULL)
        {
	    printf("FAILED AT i=%d\n",i);
            break;
        }
        else
        {
            memset(p,97,2000);
        }
    }
    exit(16);
}
