#include <stdio.h>
#include <malloc.h>


void *allocated[132000];
int num_alloced = 0;

void main()
{
   void *ptr;
   int i, amt = 0;

   for (i=0; ;i++)
   {
        if (i % 5 == 0)
        {
            if (ptr = mem_alloc(64))
	            amt += 64;
        }
        else if (i % 4 == 0)
        {
            if(ptr = mem_alloc(65536))
	            amt += 65536;
        }
        else if(i % 3 == 0)
        {
            if(ptr = mem_alloc(64))
	            amt += 64;
        }
        else
            if(ptr = mem_alloc(65536))
                amt+=65536;
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
    printf("I %d amt %d\n",i,amt);
    for (i=0; i < num_alloced; i++)
	{
		//printf("\n-----Freeing----- %d\n", i);
        mem_free(allocated[i]);
	}
    //printf("after free \n");
    amt = 0;
    num_alloced = 0;
    for (i=0; ;i++)
    {
        if (i % 5 == 0)
        {
            if (ptr = mem_alloc(64))
	            amt += 64;
        }
        else if (i % 4 == 0)
        {
            if(ptr = mem_alloc(65536))
	            amt += 65536;
        }
        else if(i % 3 == 0)
        {
            if(ptr = mem_alloc(64))
	            amt += 64;
        }
        else
            if(ptr = mem_alloc(65536))
                amt += 65536;
         if ( ! ptr)
             break;
         allocated[num_alloced++] = ptr;
     }
     printf("I %d amt %d\n",i,amt);
}
