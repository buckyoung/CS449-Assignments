#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prototypes:
void play_game();
void init_dice_arrays();
void roll_dice();
void print_dice();
char get_rand_die();
void additional_rolls();
void reset_dice(int);
void score_upper(int);
void score_lower(int);
void print_total_score();
void print_scoreboard();
int comp (const void *, const void *);
int sl_kind(int, char*);
int sl_straight(int, char*);
void unique (char*);
int sl_fullhouse(char*);
char *int_to_string(int);
void reverse(char*, int);


//Globals:
int upper_section[6];
int upper_bonus;
int lower_section[7];
//Dice Helpers
int reroll_bool[5];
char die_value[5];


//Main:
int main(){

	int i;

	//Create and Initialize Linked List of Dice:
 	init_dice_arrays(); //initialize helper arrays

 	//Initialize Upper and Lower with -1 (to support "no showing" of the score on Unscored sections)
 	for(i = 0; i < 6; i++){
 		upper_section[i] = -1;
 	}
 	for(i = 0; i < 7; i++){
 		lower_section[i] = -1;
 	}

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
 	char *user_input = malloc(sizeof(char)*10); //?


	//Game Loop:
 	while(current_turn <= 13){

 		printf("\nYour first roll:\n");
		roll_dice(); //Get dice roll
		print_dice(); //Print dice roll
 		
 		additional_rolls(); //Rerolls 2 more times if User chooses to do so
		
		// * * *  Dice are now finalized  * * * //

 		//Let the user decide how to score:
 		printf("Place dice into section:\n");
 		printf("1) Upper Section\n2) Lower Section\n");
 		printf("Selection? ");
 		fgets(user_input, sizeof(int), stdin); //get user input
 		selection = atoi(user_input);

 		printf("\nPlace dice into: \n");
 		if(selection == 1){//Upper section
 			printf("1) Ones\n2) Twos\n3) Threes\n4) Fours\n5) Fives\n6) Sixes\n");
 			printf("Selection? ");
			fgets(user_input, sizeof(int), stdin); //get user input
 			selection = atoi(user_input);
			score_upper(selection); //Handle Scoreboard
 		} else if (selection == 2) { //Lower Section
 			printf("1) Three of a kind\n2) Four of a kind\n3) Small straight\n4) Large straight\n5) Full house\n6) Yahtzee\n7) Chance\n");
			printf("Selection? ");
			fgets(user_input, sizeof(int), stdin); //get user input
 			selection = atoi(user_input);
			score_lower(selection);
 		}


		print_total_score(); //Print score
		print_scoreboard(); //Display scoreboard (upper and lower sections)

		//Loop 13 times
		current_turn++;
		//Reset all dice (Need rerolled!)
		reset_dice(1);
	}

	   free(user_input); //?

}

/*
 *  Prints scoreboard 
 */
void print_scoreboard(){
	printf("\nOnes: %s \t\t\tFours: %s\nTwos: %s\t\t\t\tFives: %s\nThrees: %s\t\t\tSixes: %s\n", (upper_section[0] == -1)?" ":int_to_string(upper_section[0]), (upper_section[3] == -1)?" ":int_to_string(upper_section[3]), (upper_section[1] == -1)?" ":int_to_string(upper_section[1]), (upper_section[4] == -1)?" ":int_to_string(upper_section[4]), (upper_section[2] == -1)?" ":int_to_string(upper_section[2]), (upper_section[5] == -1)?" ":int_to_string(upper_section[5]));
	printf("Upper Section Bonus: %d\n", upper_bonus);
 	printf("Three of a kind: %s\t\tFour of a kind: %s\nSmall straight: %s\t\tLarge straight: %s\nFull house: %s\t\t\tYahtzee: %s\nChance: %s\n", (lower_section[0] == -1)?" ":int_to_string(lower_section[0]), (lower_section[1] == -1)?" ":int_to_string(lower_section[1]), (lower_section[2] == -1)?" ":int_to_string(lower_section[2]), (lower_section[3] == -1)?" ":int_to_string(lower_section[3]), (lower_section[4] == -1)?" ":int_to_string(lower_section[4]), (lower_section[5] == -1)?" ":int_to_string(lower_section[5]), (lower_section[6] == -1)?" ":int_to_string(lower_section[6]));
}

/*
 *  Prints total score
 */
void print_total_score(){
	int i;
	int total = 0;

	//Sum Upper section scores
	for(i = 0; i < 6; i++){ //6 is max in upper section
		if(upper_section[i] > 0){  //Only add positive values (ignore initialization values)
			total += upper_section[i];
		}
	}

	//Bonus for upper section
	if(total >= 63){
		upper_bonus = 35;
		total += upper_bonus;
	}

	//Sum Lower section scores
	for(i = 0; i < 7; i++){ //7 is max in lower section
		if (lower_section[i] > 0){   //Only add positive values (ignore initialization values)
			total += lower_section[i];
		}
	}

	printf("\nYour score so far is: %d\n", total);
}

/*
 *  returns BOOLEAN -- 1 if fullhouse exists, 0 if not
 */
int sl_fullhouse(char* values){
	int i, j;
	int freq_a = 1;
	int freq_b = 0;

	char a = values[0];
	char b = 0;

	for(i = 1; i < 5; i++){
		if (values[i] == a){
			freq_a += 1;
		} else {
			if (b == 0){ //we found a new number
				b = values[i];
				freq_b = 1;
			} else if (values[i] == b){
				freq_b += 1;
			}
		}
	}

	if ((freq_a == 2 && freq_b == 3) || (freq_a == 3 && freq_b == 2)){
		return 1; //TRUE
	}

	return 0; //FALSE
}

/*
 *  (s)core (l)ower / 3,4,5 of a kind 
 *	Returns BOOLEAN -- 1 if _ of a kind exists, 0 if it does not
 */
int sl_kind(int kind, char *values){
	int run, i, j;

	for (j = 1; j <= 6; j++){ //cycle all possible dice values
		run = 0;
		for (i = 0; i < 5; i++){ //cycle all dice
			if (values[i] == j){
				run++;
			}
		}
		if (run >= kind){
			return 1; //TRUE
		}
	}

	return 0; //FALSE
}

/*
 * Returns BOOLEAN if small/large straight exists
 */
int sl_straight(int target_length, char *values){
	int i;
	int run = 1;

	unique(values); //Delete duplicates in values[]

	for (i = 0; i < target_length-1; i++){
		if(values[i+1] == (values[i]+1)){ //check if next # in sequence is one above the current i # (2,3 yeilds "if 3 == 3") || ie: check for a straight!
			run++;
		} else { //reset run
			return 0; //FALSE -- CANNOT BE A STRAIGHT
		}
	}

	if (run == target_length){
		return 1; //TRUE! Its a straight of (length)
	}

	return 0;//FALSE -- safety check

} 

/*
 *  Calculates score for LOWER section
 */
void score_lower(int selection){
	int index = selection-1;
	int total = 0;
	char values[5];
	int i;

	//Sort the dice:
	for(i = 0; i < 5; i++){
		values[i] = die_value[i];
	}
	qsort (values, 5, sizeof(char), comp); //sort array

	// * * *  Dice are now sorted in values[]  * * *

	//Determine score:
	switch(selection){
		case 1: //Three of a kind: if good, +total of all dice
			if(sl_kind(3, values)){ //if good
				for(i = 0; i < 5; i++){
					total+=die_value[i];
				}
			}
		break;

		case 2: //Four of a kind: if good, +total of all dice
			if(sl_kind(4, values)){
				for(i = 0; i < 5; i++){
					total+= die_value[i];
				}
			}
		break;

		case 3: //Small Straight: if good, +30
			if(sl_straight(4, values)){ //if four in a row
				total = 30;
			}
		break;

		case 4: //Large Straight: if good, +40
			if(sl_straight(5, values)){ //if five in a row
				total = 40;
			}
		break;

		case 5: //Full House: if good, +25
			if(sl_fullhouse(values)){
				total = 25;
			}
		break;

		case 6: //Yahtzee!: if good, +50
			if(sl_kind(5, values)){
				for(i = 0; i < 5; i++){
					total+= die_value[i];
				}
			}
		break;

		case 7: //Chance: +Total of all dice
			for(i = 0; i < 5; i++){
				total+= die_value[i];		
			}
		break;

		//Do nothing
	}

	lower_section[index] = total; //Store score
}

/*
 *  Calculates and stores score for the UPPER section
 */ 
void score_upper(int selection){
	int index = selection-1;
	int quantity = 0;
	int i;
	
	//Get quantity:
	for(i = 0; i < 5; i++){
		if (die_value[i] == selection){
			quantity++;
		}
	}

	//Calculate and store score
	upper_section[index] = selection * quantity; //Example: User has three 1's:  upper_section[0] = 1 * 3;
}

/*
 *  Resets ALL dice for rerolling if b = 1, and not rerolling if b = 0
 */
void reset_dice(int b){
	int i;
	for(i = 0; i < 5; i++){
		reroll_bool[i] = b;
	}
}

/*
 *  Handles the logic for roll 2 and 3 if the user chooses to reroll any dice
 */
void additional_rolls(){
	int j, i;
	char selection;
	char *token;
	int reroll_input[6];
	char user_input[10];

 		for(j=0; j<2; j++){
 			
 			for(i = 0; i < 6; i++){ //reset 
 				reroll_input[i] = 0;
 			}

 			printf("Which dice to reroll?\n");
 			//Get input
 			fgets(user_input, 10, stdin);

 			//Parse input to reroll array
 			i = 0;
 			token = strtok(user_input, " "); //get first token
 			while (token != NULL){
 				reroll_input[i++] = atoi(token); //string to int convert
 				token = strtok(NULL, " "); //get next token
 			}

 			//Check if user entered 0 as the first input
 			if(reroll_input[0] == 0){ //user does not want to reroll any dice
 				break; //BREAK FOR Loop
 			}

			//If input == 0, move on, else roll again//
 			i = 0; 
 			do {
 				selection = reroll_input[i++];
 				if (selection != 0){
 					reroll_bool[selection-1] = 1; //True, must reroll!
				}
			} while (selection != 0);

			//Reroll dice:
			printf("Your %s roll:\n", (j==1)?"final":"second");
			roll_dice(); //Get dice roll
			print_dice(); //Print dice roll

			
		} //End Rerollings

}

/*
 *  Returns one random die roll
 */
char get_rand_die(){
	FILE* f;
	char result; //one byte!
	int read;

	f = fopen("/dev/dice", "r");

	if (f == NULL){ //Sanity check -- should never happen!
		printf("\nERROR: Failed to open /dev/dice, returning rand() value\n");
		return ((rand()%6)+1);
	}
	
	do{ 
		//Maybe fseek to position 0?
		read = fread(&result, sizeof(char), 1, f);
	} while (read !=1); //Try to read one byte -- keep doing it until it works!


	if ( !((result <= 6) && (result >= 1)) ){ //Sanity check -- should never happen!
		printf("\nERROR: Die value read from /dev/dice was %d\n Returning rand() value\n", result);
		return ((rand()%6)+1); 
	}

	fclose(f);

	return result;
}

/*
 *  Prints the dice values
 */
void print_dice(){
	int i;
	for(i = 0; i < 5; i++){
		printf(" %d ", die_value[i]);
	}
	printf("\n\n");

}

/*
 *  Reroll if needed
 */
void roll_dice(){
	int i;
	for(i = 0; i < 5; i++){
		if(reroll_bool[i] != 0){ //!0 = True, so rerol!
			die_value[i] = get_rand_die();
		}
	}

	reset_dice(0); // none need rerolled

}

/*
 *  Initializes the helper arrays
 */
void init_dice_arrays(){
	int i;

	for (i = 0; i < 5; i++){
		reroll_bool[i] = 1; //reroll!
		die_value[i] = 0; //init to 0
	}

}

/*
 *  Comparison function for qsort
 */ 
int comp (const void * e1, const void * e2) {
   char c1 = *(char*)e1; 
   char c2 = *(char*)e2; 

    if (c1 == 0){
    	return 1;
    }
    if (c2 == 0){
    	return -1;
    }

   return c1 - c2; 
}

/*
 *  Unique function for removing duplicates
 */
void unique (char* values){
	char one, two, three, four, five;
	one = values[0];
	two = values[1];
	three = values[2];
	four = values[3];
	five = values[4];

	if (four == five){
		five = 0;
	}
	if (three == four){
		four = 0;
	}
	if (two == three){
		three = 0;
	}
	if (one == two){
		two = 0;
	}

	values[0] = one;
	values[1] = two;
	values[2] = three;
	values[3] = four;
	values[4] = five;

	qsort (values, 5, sizeof(char), comp); //sort array

}

/*
 * Implementation of Integer to String
 */
char *int_to_string(int n){
	 char *out = malloc(sizeof(char)*10);
    // if negative, need 1 char for the sign
    int sign = n < 0? 1: 0;
    int i = 0;
    if (n == 0) {
        out[i++] = '0';
    } else if (n < 0) {
        out[i++] = '-';
        n = -n;
    }
    while (n > 0) {
        out[i++] = '0' + n % 10;
        n /= 10;
    }
    out[i] = '\0';
    reverse(out + sign, i - sign);
    return out;
}
//Reverse function for i to a above ^
void reverse(char* str, int length){
    int i = 0, j = length-1;
    char tmp;
    while (i < j) {
        tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++; j--;
    }
}