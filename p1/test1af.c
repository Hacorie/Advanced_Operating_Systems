#include <stdio.h>
#include <malloc.h>
#include <string.h>

void *mem_alloc(int);
void *allocated[132000];
int num_alloced;

/****
void *mem_alloc(int nbytes)
{
    if (num_alloced < 131000)
        return malloc(nbytes);
    return NULL;
}
****/

void main()
{
    void *ptr;
    int i, amt;

    amt = 0;
    num_alloced = 0;
    for (i=0; ;i++)
    {
        if (i % 4 == 0)
        {
            ptr = mem_alloc(8);
            if (ptr)
            {
                memset(ptr,'a',8);
                amt += 8;
            }
        }
        else if (i % 3 == 0)
        {
            ptr = mem_alloc(32);
            if (ptr)
            {
                memset(ptr,'a',32);
                amt += 32;
            }
        }
        else
        {
            ptr = mem_alloc(64);
            if (ptr)
            {
                memset(ptr,'a',64);
                amt += 64;
            }
        }
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
    printf("I %d  num_alloced %d  amt %d\n",i,num_alloced,amt);  fflush(stdout);
    for (i=0; i < num_alloced; i++)
        mem_free(allocated[i]);
    amt = 0;
    num_alloced = 0;
    for (i=0; ;i++)
    {
        if (i % 4 == 0)
        {
            ptr = mem_alloc(8);
            if (ptr)
            {
                memset(ptr,'z',8);
                amt += 8;
            }
        }
        else if (i % 3 == 0)
        {
            ptr = mem_alloc(32);
            if (ptr)
            {
                memset(ptr,'a',32);
                amt += 32;
            }
        }
        else
        {
            ptr = mem_alloc(64);
            if (ptr)
            {
                memset(ptr,'a',64);
                amt += 64;
            }
        }
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
    printf("I %d amt %d\n",i,amt);
}
