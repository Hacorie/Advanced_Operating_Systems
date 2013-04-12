#include <stdio.h>
#include <malloc.h>


void *allocated[1000000];
int num_alloced = 0;
int new_alloced = 0;
int new_i = 0;
void main()
{
   void *ptr;
   int i, amt = 0;

   for (i=0; ;i++)
   {
        if (i % 5 == 0)
        {
            if (ptr = mem_alloc(8))
	            amt += 8;
        }
        else if (i % 4 == 0)
        {
            if(ptr = mem_alloc(16))
	            amt += 16;
        }
        else if(i % 3 == 0)
        {
            if(ptr = mem_alloc(32))
	            amt += 32;
        }
        else
            if(ptr = mem_alloc(64))
                amt+=64;
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
    new_alloced = num_alloced;
    new_i = i;
    printf("I %d amt %d\n",i,amt);
    for (i=0; i < num_alloced; i++)
	{
	    if( (i+1) % 3 == 0	)
        {
            if (i % 5 == 0)
            {
                mem_free(allocated[i]);
                amt -= 8;
                new_i -=1;
            }
            else if (i % 4 == 0)
            {
                mem_free(allocated[i]);
	            amt -= 16;
                new_i -= 1;
            }
            else if(i % 3 == 0)
            {
                mem_free(allocated[i]);
	            amt -= 32;
                new_i -= 1;
            }
            else
            {
                mem_free(allocated[i]);
                amt -= 64;
                new_i -= 1;
            }
            new_alloced -= 1;
        }
	}

    printf("I %d amt %d\n",new_alloced,amt);
    for (i; ;i++)
    {
        if(ptr = mem_alloc(16))
        {
            new_alloced += 1;
            amt += 16;
           // printf("%d\n", amt);
        }
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
     printf("I %d amt %d\n",new_alloced,amt);
}
