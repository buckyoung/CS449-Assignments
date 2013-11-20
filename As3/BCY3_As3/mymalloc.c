#include <stdio.h>

//Assignment 3: CS449: Nov 5, 2013
//Buck Young: bcy3

#define MAGIC_NUM 4242

//---------   Structs
struct node {
	int magic_num; //ensures this is a node
	int in_use; //Boolean in_use: 0 = false (Memory is Free) && !0 = true (Memory is In Use)
	int size; //Size of memory chunk
	struct node* next_node; //Pointer to the next node (above)
	struct node* prev_node; //Pointer to the previous node (below)
};
//---------   Function Prototypes
void* my_malloc_brk_up(int);
void* my_nextfit_malloc(int);
void my_free(void*);
void deallocate(struct node*);
struct node* coalesce(struct node*, struct node*);
struct node* nextfit_create(int);


//---------   Global Declarations
struct node *first_node;
struct node *last_node;
struct node *next_fit_node;
int bool_init_needed = 1;

//---------   My Nextfit Malloc
//1) use sbrk to scale the brk value (heap = area from _end -> brk)
//2) Utilize a linked list struct, return ptr to beginning of amt requested
//+but have the node struct in there as well
//sbrk(amt req+size of node struct)
//3) Use next fit algorithm -- if no empty space is big enough, allocate more
void* my_nextfit_malloc(int size){
	void *ptr;
	struct node* node;

	if(bool_init_needed){ //used to get things running again!
		bool_init_needed = 0; //No more init needed!
		//pointer updates
		ptr = my_malloc_brk_up(size); //This is the first ever created space
		if(ptr == NULL){ //unsuccessful
			return NULL;
		}
		//node updates
		node = ptr-sizeof(struct node); //This is the address of the first (and last, in this case) node
		first_node = node;
		last_node = node;
		next_fit_node = node; //next fit's gotta start somewhere

	} else { //if init is not needed:
		node = nextfit_create(size);
		if (node != NULL){
			ptr = node;
			ptr += sizeof(struct node);//set ptr
			//found a spot, take care of everything!
		} else { //Couldnt find a spot, create more (with sbrk)!
			//pointer updates
			ptr = my_malloc_brk_up(size); //Push up brk
			if (ptr == NULL){ //unsuccessful
				return NULL;
			}
			//node updates
			node = ptr-sizeof(struct node); //Get node-space
			last_node->next_node = node; //the very last node is getting a next node because brk went UP
			node->prev_node = last_node; //this node's previous node is the last node
			last_node = node; //update last node to this one
		}

	}

	return ptr;
}

//---------   Nextfit search
//In: size
//Out: allocated node pointer or NULL
struct node* nextfit_create(int size){
	struct node* result;
	struct node* tmp; //acts as starting point in do/while AND as a tmp node during creation
	struct node* tmp2;
	void *ptr;
	int remaining_size;

	tmp = next_fit_node;
	result = next_fit_node;

	do{
		if(!result->in_use){
			if(result->size > (size+sizeof(struct node))){ //it must be bigger than the size and space lost to a new node creation
				//WE HAVE A BINGO!
				remaining_size = result->size - size - sizeof(struct node); //avail size minus requested size minus a new node allocation

				//Allocate result
				//magic num remains unchanged
				result->in_use = 1; //TRUE
				result->size = size;
				//Next and prev remain unchanged for now

				//Create new node after
				ptr = result;   //0x804b000
				ptr += (size + sizeof(struct node));  
				
				tmp = ptr;
				tmp->magic_num = MAGIC_NUM;
				tmp->in_use = 0; //FREE!
				tmp->size = remaining_size;
				tmp->next_node = result->next_node;
				tmp->prev_node = result;
				//Update neighbors
				result->next_node = tmp;
				(tmp->next_node)->prev_node = tmp;

				next_fit_node = result; //update next fit node

				return result; //return from here!
			}
		}

		//Update for next pass
		if (result->next_node == NULL){ //end of list
			result = first_node;
		} else { //middle of list somewhere
			result = result->next_node;
		}

	}while(result != tmp);



	return NULL;
}

//---------   My Free
//Frees previously called space -- handles coalescing and brk decrementing thru call to deallocate()
void my_free(void* ptr){
	//Declarations:
	struct node* node;

	//Usage:
	//1) Search for magic number
	node = ptr-sizeof(struct node);
	if(node->magic_num == MAGIC_NUM){ //good node
		deallocate(node); //deallocate it (handles coalescing and brk decrementing)
	} else {
		printf("my_free/ERROR: Cannot locate node referred to by %p\n");
	}

}

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
		node = coalesce(node, next);
		prev = node->prev_node;
		next = node->next_node;
	}
	if ( (!(prev == NULL)) && (!(prev->in_use)) ){
		//Prev is free so we should coalesce!
		node = coalesce(prev, node);
		prev = node->prev_node;
		next = node->next_node;
	}

	//3) Check if node is up against brk
	if( next == NULL ){ 
		//Nothing after, so move brk to beginning of this node
		sbrk( -(node->size + sizeof(struct node)) ); //set to its current location -size and -stuctsize
		
		if (first_node == last_node){
			bool_init_needed = 1; //need init on next pass!
			first_node = NULL;
			last_node = NULL;
			next_fit_node = NULL;

		} else {
			//Adjust last node
			last_node = prev;
			//Adjust last nodes next_node
			last_node->next_node = NULL;
		}
	}

}

//--------   Coalesce Next
//In: two free nodes
//Out: node which is an amalgamation of node and next
struct node* coalesce(struct node* node, struct node* next){
	struct node* tmp;

	//Construct node:
	//in use stays the same
	node->size = node->size + next->size + sizeof(struct node); //two sizes plus the size of the node!
	//ptr stays the same
	node->next_node = next->next_node;
	//prevnode stays the same

	//Invalidate the consumed node
	next->magic_num = 0;

	//Change next's next node to point to node
	tmp = next->next_node;

	//end of list check
	if(next == last_node){
		last_node = node;
	} else { //not end of list, so tmp is not null!
		tmp->prev_node = node;
	}

	return node;
}

//--------   My Malloc Brk Up
//Allocates memory by pushing brk up -- nothing fancy, no memory management algorithms here.
void *my_malloc_brk_up(int size){
	//Move the brk
	void *old_brk;
	old_brk = (void *)sbrk(size+sizeof(struct node)); //Size = Requested + Node

	if (old_brk < 0){ //sbrk returns -1 if unsuccessful
		return NULL; //unsuccessful!
	}
	//Initialize Node at beggining of memory region
	struct node* new_node; 
	new_node = old_brk; //node pointer address becomes old_brk
	new_node->magic_num = MAGIC_NUM; //
	new_node->in_use = 1; //TRUE, mem in use
	new_node->size = size; //Requested size
	new_node->next_node = NULL; //Next and Prev are determined elsewhere (my_nextfit_malloc())
	new_node->prev_node = NULL;

	return old_brk+sizeof(struct node); //Beginning of requested space
}