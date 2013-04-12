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
        if (i % 4 == 0)
        {
            if (ptr = mem_alloc(8))
	            amt += 8;
        }
        else if (i % 3 == 0)
        {
            if(ptr = mem_alloc(32))
	            amt += 32;
        }
        else
        {
            if(ptr = mem_alloc(64))
	            amt += 64;
        }
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
         if (i % 4 == 0)
         {
             if(ptr = mem_alloc(8))
	             amt += 8;
         }
         else if (i % 3 == 0)
         {
			if(ptr = mem_alloc(32))
				amt += 32;
         }
         else
         {
             if(ptr = mem_alloc(64))
				amt += 64;
         }
         if ( ! ptr)
             break;
         allocated[num_alloced++] = ptr;
     }
     printf("I %d amt %d\n",i,amt);
}
