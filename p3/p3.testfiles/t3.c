#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *s, input_line[100];

    if ( ! isatty(STDIN_FILENO))
    {
        printf("stdin must be a tty\n");
        exit(-1);
    }

    setbuf(stdout,NULL);
    printf("rbutler_sh> ");
    while (fgets(input_line,100,stdin) != NULL)
    {
        if (s = strchr(input_line,'\n'))
            *s = '\0';
        printf("INPUT_LINE: %s\n",input_line);
        printf("rbutler_sh> ");
    }
    return 0;
}
