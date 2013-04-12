#include <stdio.h>
#include <malloc.h>


//this test creates 64 byte chunks, then frees a lot of them.
//It then puts in 30k byte chunks until it cannot anymore.

void *allocated[132000];
int num_alloced = 0;

void main()
{
   void *ptr;
   int i, amt = 0;
   int new_alloced;
   int j = 0;

   for (i=0; ;i++)
   {
        if (ptr = mem_alloc(64))
	            amt += 64;
        if ( ! ptr)
            break;
        allocated[num_alloced++] = ptr;
    }
    new_alloced = num_alloced;
    printf("I %d amt %d\n",i,amt);
    for (i=2000; i < num_alloced/2; i++)
	{
		//printf("\n-----Freeing----- %d\n", i);
        mem_free(allocated[i]);
        new_alloced-=1;
        amt -= 64;
	}
    printf("I after free: %d amt after free: %d\n", new_alloced, amt);
    for(j; ; j++)
    {
        if(ptr = mem_alloc(30000))
        {
            amt += 30000;
            allocated[num_alloced++] = ptr;
            new_alloced +=1;
        }
        if(!ptr)
            break;
    }
    printf("I %d amt %d\n",new_alloced,amt);
    for(i = 0; i < num_alloced; i++)
        mem_free(allocated[i]);
}
