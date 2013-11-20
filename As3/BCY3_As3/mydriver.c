#include <stdio.h>
#include "mymalloc.h"
#include <string.h>

//Test driver for my_nextfit_malloc and my_free
//Assignment 3: CS449: Nov 5, 2013
//Buck Young: bcy3

int main (void){ 
	//Declarations
	void *ptr1;
	void *ptr2;
	void *ptr3;
	char *string;
	int *array;
	void *brk;
	int i;

	//Usage
	printf("\n");
	brk = (void *)sbrk(0);
	printf("Starting BRK location: %p\n", brk);
	printf("\n- - - - -|  First Allocation  |- - - - -\n");
	printf("Allocating ptr1, size 100\n");
	ptr1 = my_nextfit_malloc(100);
	printf("Allocating ptr2, size 200\n");
	ptr2 = my_nextfit_malloc(200);
	printf("Allocating ptr3, size 300\n");
	ptr3 = my_nextfit_malloc(300);

	printf("\n** BRK location: %p\n**    Should be: %p   (assuming sizeof(struct node)=20)\n", sbrk(0), (brk+600+20*3));
	
	printf("\n- - - - -|  Write and Read  |- - - - -\n");

	printf("Writing 99 chars to ptr1 string.\n");
	string = ptr1;
	for(i=0; i<99; i++){
		string[i] = (i%26)+65;
	}
	string[99] = '\0';
	printf("Printing: %s\n", string);

	printf("\nWriting \"buck\" to ptr2 string.\n");
	string = ptr2;
	strcpy(string, "buck");
	printf("Printing: %s\n", string);

	printf("\nWriting 300 ints to ptr3 array.\n");
	array = ptr3;
	for(i=0; i<300; i++){
		array[i] = i+1;
	}
	printf("Printing: ");
	for(i=0; i<300; i++){
		printf("%d ",array[i]);
	}
	printf("\n");

	printf("\n** BRK location: %p\n**    Should be: %p   (assuming sizeof(struct node)=20)\n", sbrk(0), (brk+600+20*3));
	

	printf("\n- - - - -|  Freeing  |- - - - -\n");


	printf("Calling free on ptr1.\n");
	my_free(ptr1); 

	printf("Calling free on ptr2.\n");
	my_free(ptr2); 

	printf("Calling free on ptr3.\n");
	my_free(ptr3); 

	printf("\n** BRK location: %p\n**    Should be: %p   (assuming sizeof(struct node)=20)\n", sbrk(0), brk);
	
	printf("\n- - - - -|  Testing NextFit  |- - - - -\n");
	printf("Many calls to my malloc and free...\n\n");
	
	ptr1 = my_nextfit_malloc(132);
	printf("ptr1 allocated at: %p, size 132\n", ptr1);

	printf("Allocation at: %p, size 143\n", my_nextfit_malloc(143));
	
	ptr2 = my_nextfit_malloc(154);
	printf("ptr2 allocated at: %p, size 154\n", ptr2);

	printf("Freeing ptr1 from: %p, size 132\n", ptr1); 
	my_free(ptr1);

	ptr3 = my_nextfit_malloc(10);
	printf("ptr3 allocated at: %p, size 10\n", ptr3);

	printf("Allocation at: %p, size 20\n", my_nextfit_malloc(20));
	printf("Allocation at: %p, size 112\n", my_nextfit_malloc(112));

	printf("Freeing ptr3 from: %p, size 10\n", ptr3); 
	my_free(ptr3);

	printf("Freeing ptr2 from %p, size 154\n", ptr2); 
	my_free(ptr2);

	printf("Allocation at: %p, size 143\n", my_nextfit_malloc(143));

	ptr1 = my_nextfit_malloc(23);
	printf("ptr1 allocated at: %p, size 23\n", ptr1);

	ptr3 = my_nextfit_malloc(266);
	printf("ptr3 allocated at: %p, size 266\n", ptr3);

	printf("Freeing ptr1 from %p, size 23\n", ptr1); 
	my_free(ptr1);

	printf("\n** BRK location: %p\n**    Should be: %p   (assuming sizeof(struct node)=20)\n", sbrk(0), (brk+1070));
		
	printf("\nFinished.\n");
}