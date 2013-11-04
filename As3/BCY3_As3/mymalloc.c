//11.4.2013 Todo: ok, i have a lot of base functionality down for inititalizing the linked list and pointer tables
//and adding a few elements to them. 

//TODO: Well, i can start working on free -- I gotta sort the pointer table then binary search for the pointer
//If it is on the list, start removing that shit --- i have the comp and qsort ready and waiting for this...

//TODO: Well, lets see... the else in my malloc needs another condition (it now unconditional does a dumb brk up) -- 
//when free is implemented i will need to start considering other cases.

//TODO: free needs to bring down sbrk if its at the end of the list...

//TODO: OHHHH !!!  Well I have this major issue where i need my pointer table to be dynamically sized.... so I guess
//maybe during init, I can have a pointer to a bunch of mymallocedbrkup space for it -- and a current max size
//if it gets close to that max size ever, then double it and remalloc some space AND free the space that the array
//once held.. it will make sense  and it is very important. so technically, the space for the pointer table will be
//my first node -- its handled by ME and the user should not be able to call free on it (so i may need a "private"
//implementation of free for this purpose? maybe not.)... no definitely not because the user wont ever really get that 
//memory address -- k bye now :D

#include <stdio.h>

//---------   Linked List Node Struct
struct node {
	int in_use; //Boolean in_use: 0 = false (Memory is Free) && !0 = true (Memory is In Use)
	int size; //Size of memory chunk
	struct node* node_ptr;
	struct node* next_node; //Pointer to the next node (above)
	struct node* prev_node; //Pointer to the previous node (below)
};
//---------   Prototypes and declarations
void *pointer_table[100]; //Declare a pointer table of size 100 which accepts requested memory pointers (supports free)
size_t table_size; //size_t is unsigned int, guaranteed to hold an array size
struct node *first_node;
struct node *last_node;

//Allocates memory by pushing brk up -- nothing fancy, no memory management algorithms here.
void *my_malloc_brk_up(int size){
	//Move the brk
	void *old_brk;
	old_brk = (void *)sbrk(size+sizeof(struct node)); //Size = Requested + Node

	//Initialize Node at beggining of memory region
	struct node* new_node; 
	new_node = old_brk; //node pointer address becomes old_brk
	new_node->in_use = 1; //TRUE, mem in use
	new_node->size = size; //Requested size
	new_node->node_ptr = new_node; //Save "this" address
	//Determine next and prev nodes!
	new_node->next_node = NULL; // TODO
	new_node->prev_node = NULL; // TODO
	printf("MyMallocBrkUp: About to return address:%p  |  old_brk:%p\n", old_brk+sizeof(struct node), old_brk); //DEBUG

	return old_brk+sizeof(struct node); //Beginning of requested space
}

//---------   My Malloc
//1) use sbrk to scale the brk value (heap = area from _end -> brk)
//2) Utilize a linked list struct, return ptr to beginning of amt requested
//+but have the node struct in there as well
//sbrk(amt req+size of node struct)
//3) Use next fit algorithm -- if no empty space is big enough, allocate more
void* my_nextfit_malloc(int size){
	
	void *ptr;
	struct node* node;

	//Static variable --> *  Initializing the Linked List  * <--
	static int bool_init_needed = 1; //True, initialized is needed
	if(bool_init_needed){
		bool_init_needed = 0; //No more init needed!
		//pointer updates
		ptr = my_malloc_brk_up(size); //This is the first ever created space
		pointer_table[0] = ptr; //first ever pointer
		table_size = 1;
		//node updates
		node = ptr-sizeof(struct node); //This is the address of the first (and last, in this case) node
		first_node = node;
		last_node = node;

	} else { //if init is not needed:
		//pointer updates
		ptr = my_malloc_brk_up(size); //Push up brk
		pointer_table[table_size++] = ptr; //Add pointer to table and increment size
		//node updates
		node = ptr-sizeof(struct node); //Get node-space
		last_node->next_node = node; //the very last node is getting a next node because brk went UP
		node->prev_node = last_node; //this node's previous node is the last node
		last_node = node; //update last node to this one

	}

	//IF NEXTFIT DOES NOT FIND A USEABLE LOCATION:
	//nextfit(size+sizeof(struct node));

 	//Brand new allocation at the top of the heap:
	//int* old_brk = sbrk(0); //Do I need this?
	//int* new_brk = sbrk(size+sizeof(struct node)) + sizeof(struct node); //Scale by amount req + sizeof node, and store the point after the node (beginning of requested space)
	return ptr;
}

int main (void){ //DEBUG MAIN DEBUG -- keep this debug for the TEST-DRIVER.c
	printf("\nAdding First Node\n");
	void *ptr = my_nextfit_malloc(100);
	struct node* node;
	node = ptr-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[0]:%p,\n", first_node, last_node, pointer_table[table_size-1]);

	printf("\nAdding Second Node\n");
	ptr = my_nextfit_malloc(200);
	node = ptr-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[1]:%p,\n", first_node, last_node, pointer_table[table_size-1]);

	printf("\nAdding Third Node\n");
	ptr = my_nextfit_malloc(300);
	node = ptr-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[2]:%p,\n", first_node, last_node, pointer_table[table_size-1]);
}

//---------   My Free
void my_free(void* ptr);
//	 qsort : http://stackoverflow.com/questions/1787996/c-library-function-to-do-sort
//	... use qsort to SORT the POINTER TABLE before SEARCHING it to find if the pointer has been allocated by a previous call to mymalloc!!!

//&&&& qsort (pointer_table, table_size(-1??), sizeof(*pointer_table??), comp);

//1) deallocates a ptr that was originally allocated by my malloc above -- NEED SOME KIND OF TABLE...
//2) Coalesce adjacent free blocks
//3) If the block that touches brk is free, then use sbrk with a negative offset to reduce the size of the heap

//---------   Compare function for qsort
int comp (const void *a, const void *b) 
{
    int x = *((int*)a);
    int y = *((int*)b);
    
    return (x > y) - (x < y);
}
