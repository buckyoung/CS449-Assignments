#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structs:
typedef struct die_node{
 	struct die_node* next; // Next node
	short reroll_bool;     // 0=False, !0=True
	short die_number;      // 1-5 
	short die_value;       // 1-6
} die_node; 

//Prototypes:
void play_game();
void init_nodes(die_node*);
void roll_dice(die_node*);
void print_dice(die_node*);
short get_rand_die();
void additional_rolls();

//Globals:
die_node* first_node;

//Main:
int main(){

	//Create and Initialize Linked List of Dice:
 	first_node = malloc(sizeof(die_node) * 5);
 	init_nodes(first_node); //initialize linked list

 	//Start the fun!
	play_game();

	return 0;
}

/*
 *  Game Loop
 */
 void play_game(){

	//Initializations:
 	int total_score = 0;
 	int current_turn = 1;
 	int i, j;
 	int selection;
 	die_node* node;
 	

	//Game Loop:
 	while(current_turn <= 13){

 		printf("Your first roll:\n");
		roll_dice(first_node); //Get dice roll
		print_dice(first_node); //Print dice roll
 		
 		additional_rolls(); //Rerolls 2 more times if User chooses to do so

		// ********  Dice are now finalized

 		printf("Place dice into section:\n");
 		printf("1) Upper Section\n2) Lower Section\n");
 		printf("Selection? ");

		//Get input//

 		printf("Place dice into: ");
		//If upper//
 		printf("1) Ones\n2) Twos\n3) Threes\n4) Fours\n5) Fives\n6) Sixes\n");
		//Get input//
		//Else if Lower//
 		printf("1) Three of a kind\n2) Four of a kind\n3) Small straight\n4) Large straight\n5) Full house\n6) Yahtzee\n7) Chance\n");
		//Get input//

		//Do scoring logic//

		printf("Your score so far is: %d\n", total_score); //Print score
		//Display scoreboard//

		//Loop 13 times//
		current_turn++;
	}

}

/*
 *  Handles the logic for roll 2 and 3 if the user chooses to reroll any dice
 */
void additional_rolls(){
	int j, i, selection;
	int reroll_input[6];
	die_node* node;

 		for(j=0; j<2; j++){
 			printf("Which dice to reroll? [Enter one at a time] [0 when done]\n");
 			//Parse input
 				i = 0;
 				do{
 					scanf("%d", &selection);
 					reroll_input[i++] = selection;
 				} while (selection != 0);

 			//Check if user entered 0 as the first input
 			if(reroll_input[0] == 0){ //user does not want to reroll any dice
 				break; //BREAK FOR Loop
 			}

			//If input == 0, move on, else roll again//
 			i = 0; 
 			do {
 				selection = reroll_input[i++];
	
				//parse thru linked list of die
 				if (selection != 0){
					node = first_node;//init before while
					while (node != NULL){
						if (node->die_number == selection){
							node->reroll_bool = 1; //True, need to reroll!
						}
						node = node->next;
					}
				}
			} while (selection != 0);

			//Reroll dice:
			printf("Your %s roll:\n", (j==2)?"final":"second");
			roll_dice(first_node); //Get dice roll
			print_dice(first_node); //Print dice roll

		} //End Rerollings
}

/*
 *  Returns one random die roll
 */
short get_rand_die(){
	return ((rand() % 6) + 1);
}

/*
 *  Prints the dice values
 */
void print_dice(die_node* node){
	
	while (node != NULL){
		printf(" %d ", node->die_value);
		node = node->next;
	}
	printf("\n");

}

/*
 *  Fills the linked list with dice rolls
 */
void roll_dice(die_node* node){

	while (node != NULL){
		if(node->reroll_bool != 0){ //!0 = True, so reroll it!
			node->die_value = get_rand_die();
			node->reroll_bool = 0; //Set to not reroll
		}
		node = node->next;
	}

}

/*
 *  Initializes a linked-list of 5 dice
 */
void init_nodes(die_node* node){
	short die_number;
	void* next;

	for (die_number = 1; die_number<=5; die_number++ ){
		next = node+sizeof(die_node);
		node->next = next;
		node->reroll_bool = 1;
		node->die_number = die_number;
		node->die_value = 0;
		node = next;
	}
	node = node-sizeof(die_node);
	node->next = NULL;

}