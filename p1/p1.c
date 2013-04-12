/*
Programmer: Nathan Perry
Class: Advanced OS
Project: 1
Description: Implementation of Malloc and Free
*/

#include <stdio.h>
#include <malloc.h>

#define MAX_SIZE     256*1024

//This is a structure that will act like a linked list
typedef struct malloc_stc
{
    struct malloc_stc *next;
    unsigned int size;
    void *data;
} malloc_stc;

//function prototypes
void *mem_alloc(int nbytes);
void mem_free(void* ptr);

void* get_mem_from_OS();


void *data_arr[4] = {NULL};	//holds up to 4 chunks of 256k
malloc_stc *head[4] = {NULL};	//pointers to the beginning of each linked list

malloc_stc *curr; //a pointer that will help with traversing the lists
static int call_count = 0;

//mem_alloc makes calls to get_mem_from_OS() in dr. butler's library
//Returns a pointer to where a chunk of data is inserted
void* mem_alloc(int nbytes)
{
    int i = 0;	//Every program needs an i
    int done;	//flag variable for loop control
    unsigned int freeSize;	//variable to dictate how much freeSize is left in a chunk
    malloc_stc *newNode = malloc(sizeof(malloc_stc));	//make a new node for insertion
    unsigned int nextStart;		//Variable to hold the place where the next item should be inserted

    //Error Checking
    if(nbytes <= 0 || nbytes >= 256*1024)
        return NULL;

	//loop for the four possible chunks
    for(i = 0;  i<4; i++ )
    {
		//if no chunk has been allocated, call get_mem_from_OS
        if(data_arr[i] == NULL)
        {
            data_arr[i] = get_mem_from_OS();
            if(!data_arr[i])
                return NULL;

			//Update newNode and insert it in the list and return it's memory ptr
            newNode->size = nbytes;
            newNode->data = data_arr[i];
            newNode->next = NULL;
            head[i] = newNode;

            return head[i]->data;
        }
        else	//if a chunk already exists in this i-value
        {
			//if the chunk has been allocated, but the list is empty
            if(head[i] == NULL)
            {
				//update the node and insert it as the head
                newNode->size = nbytes;
                newNode->data = data_arr[i];
                newNode->next = NULL;
                head[i] = newNode;

				//return address of this newly inserted item
                return head[i]->data;
            }
            else	//if the chunk has been allocated and it has items in it
            {
				//check and see if there is free space before the head
                freeSize = (unsigned int)head[i]->data - (unsigned int)data_arr[i];
				
				//if you want to put in an amount that will fit before the head
				//make it your new head.
                if(nbytes <= freeSize)
                {
                    newNode->size = nbytes;
                    newNode->data = data_arr[i];
                    newNode->next = head[i];
                    head[i] = newNode;

                    //return address to new Head
                    return head[i]->data;
                }
                else	//if it has to go after the head
                {
                    curr = head[i];		//prime loop
                    done = 0;			//set flag
					
					//loop over the linked list
					while (curr != NULL && !done) 
					{
						if (curr->next == NULL) 
						{
							//calculate how much free space remains
							freeSize = ((unsigned int)data_arr[i] + MAX_SIZE) - ((unsigned int)curr->data + curr->size);
							
							//if the amount you want to insert fits, then put it in the list
							if ( nbytes <= freeSize ) 
							{
								newNode->data = (void*)((unsigned int)curr->data + curr->size); 
								newNode->size = nbytes;
								newNode->next = NULL;
								curr->next = newNode;

								//return the memory address to the new item
								return newNode->data;
							}
							else //it cannot fit in this memory block, so exit out of while loop and enter next iteration of for loop 
							{
								done = 1;
							}
						}
						else //if the next item in the list is not Null, check and see if there is room inbetween here and there 
						{
							//calculate how much space is inbetween if any
							freeSize = ((unsigned int)curr->next->data - ((unsigned int)curr->data + curr->size));
							
							//if the amount you want to insert fits, then put it in and update linked list
							if (nbytes <= freeSize) 
							{
								newNode->data = (void*)((unsigned int)curr->data + curr->size);
								newNode->size = nbytes;
								newNode->next = curr->next;
								curr->next = newNode;

								//return memory address of inserted item
								return newNode->data;
							}
							else 
							{
								curr = curr->next;
							}
						}
                    }
                }
            }
       }
    }

    return NULL;
}


//mem_free gets rid of data the user does not want taking up memory anymore.
//calls free() to delete user defined data structure
void mem_free(void *ptr)
{
    int i;
    malloc_stc* freeThis;	//temp pointer
	
	//loop over every chunk
    for(i = 0; i < 4; i++)
    {
		//if the chunk is empty, try the next chunk
        if(head[i] == NULL)
            continue;
        else	//if the chunk is not empty
        {
			// prime loop
            curr = head[i];
			
			//if the head is the item you are looking for, free it, update head, and return
            if(curr->data == ptr)
            {
                head[i] = curr->next;
                free(curr);
                return;
            }
			
			//loop over the list and try and find the data you are looking for
            for(; curr->next != NULL; curr = curr->next)
            {
				//if you find the item, set a temp ptr, then reorganize the list and free the temp
                if(curr->next->data == ptr)
                {
                    freeThis = curr->next;

                    curr->next = curr->next->next;
                    free(freeThis);
                    return;
                }
            }
        }
    }
    return;
}

// SAMPLE CODE - MAY CHANGE ***************************

#include <stdio.h>
#include <malloc.h>

void *get_mem_from_OS()
{
    void *ptr;

    if (call_count >= 3)
        return NULL;
    call_count++;
    ptr = malloc(256 * 1024);
    // turn on the next line during grading to see if really using my code
    // printf("os retrieved block of mem %d at %p\n",call_count,ptr);
    return ptr;
}
