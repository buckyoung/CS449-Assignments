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
#include <stdlib.h>

//---------   Structs
struct node {
	int in_use; //Boolean in_use: 0 = false (Memory is Free) && !0 = true (Memory is In Use)
	int size; //Size of memory chunk
	struct node* node_ptr;
	struct node* next_node; //Pointer to the next node (above)
	struct node* prev_node; //Pointer to the previous node (below)
};
//---------   Function Prototypes
void* my_malloc_brk_up(int);
void* my_nextfit_malloc(int);
void my_free(void*);
int comp(const void *, const void *); 
int find_in_table(void*);
void deallocate(struct node*);
struct node* coalesce(struct node*, struct node*);

//---------   Global Declarations
void *pointer_table[100]; //Declare a pointer table of size 100 which accepts requested memory pointers (supports free)
size_t table_size; //size_t is unsigned int, guaranteed to hold an array size
struct node *first_node;
struct node *last_node;

//--------    Debug main, save for later (driver)
int main (void){ //DEBUG MAIN DEBUG -- keep this debug for the TEST-DRIVER.c
	//Declarations
	void *ptr1;
	void *ptr2;
	void *ptr3;
	struct node* node;
	int i;

	//Usage
	printf("\nAdding First Node\n");
	ptr1 = my_nextfit_malloc(100);
	node = ptr1-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[0]:%p,\n", first_node, last_node, pointer_table[table_size-1]);

	printf("\nAdding Second Node\n");
	ptr2 = my_nextfit_malloc(200);
	node = ptr2-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[1]:%p,\n", first_node, last_node, pointer_table[table_size-1]);

	printf("\nAdding Third Node\n");
	ptr3 = my_nextfit_malloc(300);
	node = ptr3-sizeof(struct node);
	printf("Main: Node created with address:%p \n", node);
	printf("Main: InUseBool: %d, Size: %d, Ptr: %p, Next: %p, Prev: %p \n", node->in_use, node->size, node->node_ptr, node->next_node, node->prev_node); //DEBUG
	printf("Main: FirstNode:%p, LastNode:%p, pointer_table[2]:%p,\n", first_node, last_node, pointer_table[table_size-1]);

	printf("\n--------------------\n");
	printf("Beginning Free\nHere is the pointer table so far:\n");
	for(i = 0; i < table_size; i++){
		printf("%d)  %p\n", i, pointer_table[i]);
	}
	printf("First node: %p  |  Last node: %p\n", first_node, last_node);
	//print node chain
	printf("Node chain: ");
	node = first_node;
	i = 0;
	while(node != NULL){
		printf(" -> [%d. (%s | %d)]", i++, (node->in_use)?"USED":"FREE", node->size);
		node = node->next_node;
	}
	printf("\n");

	printf("\nCalling free on ptr1:\n");
	my_free(ptr1); 

	for(i = 0; i < table_size; i++){
		printf("%d)  %p\n", i, pointer_table[i]);
	}
	printf("First node: %p  |  Last node: %p\n", first_node, last_node);
	//print node chain
	printf("Node chain: ");
	node = first_node;
	i = 0;
	while(node != NULL){
		printf(" -> [%d. (%s | %d)]", i++, (node->in_use)?"USED":"FREE", node->size);
		node = node->next_node;
	}
	printf("\n");

	printf("\nCalling free on ptr3:\n");
	my_free(ptr3); 

	for(i = 0; i < table_size; i++){
		printf("%d)  %p\n", i, pointer_table[i]);
	}
	printf("First node: %p  |  Last node: %p\n", first_node, last_node);
	//print node chain
	printf("Node chain: ");
	node = first_node;
	i = 0;
	while(node != NULL){
		printf(" -> [%d. (%s | %d)]", i++, (node->in_use)?"USED":"FREE", node->size);
		node = node->next_node;
	}
	printf("\n");
}

//---------   My Nextfit Malloc
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

//---------   My Free
void my_free(void* ptr){
	//Declarations:
	int index;
	struct node* node;

	//Usage:
	//1) Must sort and search the pointer table for a memory address previously called by my_nextfit_malloc()
	//Sort
	qsort(pointer_table, table_size, sizeof(void *), comp);
	//Search
	index = find_in_table(ptr); //will return index... could be at any index from 0 to end of array (OR -1 if not found)
	
	//Found?
	if(index >= 0){ //found, index = location
		printf("Pointer found in pointer_table! At index: %d\n", index); //DEBUG
		//Found! Must deallocate...
		node = pointer_table[index]-sizeof(struct node); //get node controlling this region
		deallocate(node); //deallocate it (handles coalescing and brk decrementing)
		//Remove from ptr table and decrement table size
		pointer_table[index] = pointer_table[table_size-1]; //move null value to the end of the table
		table_size--;

	} else { //not found, index = -1
		//Do nothing
	}

}
//	 qsort : http://stackoverflow.com/questions/1787996/c-library-function-to-do-sort
//	... use qsort to SORT the POINTER TABLE before SEARCHING it to find if the pointer has been allocated by a previous call to mymalloc!!!

//&&&& qsort (pointer_table, table_size(-1??), sizeof(*pointer_table??), comp);

//1) deallocates a ptr that was originally allocated by my malloc above -- NEED SOME KIND OF TABLE...
//2) Coalesce adjacent free blocks
//3) If the block that touches brk is free, then use sbrk with a negative offset to reduce the size of the heap

//--------   Deallocate space 
//Simple deallocation, coalescing management, and brk decrementing if necessary
void deallocate(struct node* node){
	struct node* prev;
	struct node* next;
	prev = node->prev_node;
	next = node->next_node;

	//1) Free up node
	node->in_use = 0; //False, not in use; "Free"

	//2) Coalesce prev and next
	if ( (!(next == NULL)) && (!(next->in_use)) ){
		//Next is free so we should coalesce!
		printf("Deallocate: next is free, coalescing node and next\n"); //DEBUG
		node = coalesce(node, next);
		prev = node->prev_node;
		next = node->next_node;
	}
	if ( (!(prev == NULL)) && (!(prev->in_use)) ){
		//Prev is free so we should coalesce!
		printf("Deallocate: prev is free, coalescing prev and node\n"); //DEBUG
		node = coalesce(prev, node);
		prev = node->prev_node;
		next = node->next_node;
	}

	//3) Check if node is up against brk
	if( next == NULL ){ 
		printf("Deallocate: node is up against brk. decrementing brk\n"); //DEBUG
		printf("Deallocate: Current Brk Position (before deallocation):%p\n", (void*)sbrk(0)); //DEBUG
		//Nothing after, so move brk to beginning of this node
		sbrk( -(node->size + sizeof(struct node)) ); //set to its current location, -size and -stuctsize
		printf("Deallocate: Brk Position (after):%p\n", (void*)sbrk(0)); //DEBUG
		//Adjust last node
		last_node = prev;
		//Adjust prev's next_node
		prev->next_node = NULL;
	}

}

//--------   Coalesce
//In: two free nodes
//Out: node which is an amalgamation of node and next
struct node* coalesce(struct node* node, struct node* next){
	//Construct node:
	//in use stays the same
	node->size = node->size + next->size + sizeof(struct node); //two sizes plus the size of the node!
	//ptr stays the same
	node->next_node = next->next_node;
	//prevnode stays the same

	//end of list check
	if(next == last_node){
		last_node = node;
	}

	return node;
}

//--------   My Malloc Brk Up
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
	new_node->next_node = NULL; //Next and Prev are determined elsewhere (my_nextfit_malloc())
	new_node->prev_node = NULL;
	printf("MyMallocBrkUp: About to return address:%p  |  old_brk:%p\n", old_brk+sizeof(struct node), old_brk); //DEBUG

	return old_brk+sizeof(struct node); //Beginning of requested space
}

//---------   Compare function for qsort
int comp (const void *a, const void *b) {
    int x = *((int*)a);
    int y = *((int*)b);
    
    return (x > y) - (x < y);
}

//---------   Find ptr in pointer table using Binary Search
//Returns [LOCATION] if found, returns -1 if not found
int find_in_table(void *ptr){
	int index = -1; //will return -1 if not found
	int first, last, mid;

	//binary search:
	first = 0;
	last = table_size-1;
	mid = (first+last)/2;

	while (first <= last){

		if (pointer_table[mid] < ptr){
			first = mid + 1;    
		} else if (pointer_table[mid] == ptr) {
			index = mid; //assign index if found!
			break;
		} else {
			last = mid - 1;
		}

		mid = (first + last)/2;
	} 
	//end binary search

	return index;
}
