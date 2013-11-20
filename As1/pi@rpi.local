#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Constants:
#define MAX 10 //scissors = 8,   \n = 9,    \0 = 10

//Prototypes:
void play(void);
int playRound(void);
int winner(char, char);
int results(int);
void playAgain(void);

//Declarations:
char compChoice[MAX];
int userScore, compScore, roundScore;



//START of program
int main(void)
{
	char input[3];
	//Seed the rando num
	srand((unsigned int)time(NULL));

	//Welcome the user and ask if they would like to play
	printf("Welcome to Rock, Paper, Scissors \nYou will be playing a best-of-5 (first to three wins) against the computer\n");
	while ( strcmp(input, "yes") != 0 && strcmp(input, "no") != 0 )
	{
		printf("Would you like to play? yes or no: ");
		scanf("%s", input);
		//Get yes or no
		//fgets(userInput, MAX, stdin);
		//get rid of newline on user input
		//userInput[strlen(userInput) - 1] = '\0';
	}
	//If the user wants to not play
	if (strcmp(input, "no") == 0)
	{
		printf("\nToo bad...\n");
		
	}
	else //else play the game
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //"clears" 
 		play();
	}

	return 0; //EXIT THE PROGRAM

}

void play(void){
		int result;
		result = 0;

		while(result == 0){
			roundScore = playRound();
			printf("\nThe computer chose %s.", compChoice);
			result = results(roundScore);
			
		}


		playAgain();
		
}

//gets user input and computers choice
//returns >> -1:computer wins -- 0:tie -- 1:userwins
int playRound(void)
{
	int value;
	char input[8];

	input[0] = '0'; //reset 

	//Get user choice
	while ( (strcmp(input, "rock")!=0) && (strcmp(input, "paper")!=0) && (strcmp(input, "scissors")!=0) )
	{
		printf("\n\nWhat is your choice? rock paper or scissors: ");
		//fgets(userInput, MAX, stdin);
		//get rid of newline on user input
		//userInput[strlen(userInput) - 1] = '\0';
		scanf("%s", input);

	}

	
	//Get the computer choice
	value = rand() % (3 - 1 + 1) + 1; //rand: 1 or 2 or 3

	switch(value)
	{
		case 1:
			strcpy(compChoice, "rock");
			break;
		case 2:
			strcpy(compChoice, "paper");
			break;
		default: //case 3
			strcpy(compChoice, "scissors");
			break;
	}

	return winner(input[0], compChoice[0]);

}

//returns >> -1:computer wins -- 0:tie -- 1:userwins
int winner(char user, char computer)
{
	
	switch(user)
	{
		case 'r': //rock
			if (computer == 'r')
			{
				return 0; //tie
			} else if (computer == 'p')
			{
				return -1; //rock is covered by paper
			} else 
			{
				return 1; //rock beats scissors
			}
			break; //redundant

		case 'p': //paper
			if (computer == 'r')
			{
				return 1; //paper covers rock
			} else if (computer == 'p')
			{
				return 0; //tie
			} else 
			{
				return -1; //paper is cut by scissors
			}
			break; //redundant
		case 's': //scissors
			if (computer == 'r')
			{
				return -1; //scissors are beaten by rock
			} else if (computer == 'p')
			{
				return 1; //scissors cut paper
			} else 
			{
				return 0; //tie
			}
			break; //redundant
		default:
			printf("\nThere was a problem. Please provide your choice again\n");
			return playRound();
			break;
	}


}

//returns -1 if comp won the GAME, 0 if game is ongoing and, 1 if user won the game
int results(int num)
{

	if (num == -1) //computer won
	{
		printf("  YOU LOSE!  ");
		compScore+=1;
	} else if (num == 1) //user won
	{
		printf("  YOU WIN!  ");
		userScore+=1;
	} else {//else tie
		printf("  TIE!  ");
	}

	printf("\nThe score is now You: %d  Computer: %d", userScore, compScore);

	if (compScore == 3)
	{
		userScore = 0;
		compScore = 0;
		printf("\n\n\nCOMPUTER WON BEST OF 5!\n");
		return -1;
	} else if (userScore == 3){
		userScore = 0;
		compScore = 0;
		printf("\n\n\nYOU WON BEST OF 5!\n");
		return 1;
	} else {
		return 0;
	}


}


void playAgain(void){

	char input[3];

	while ( strcmp(input, "yes") != 0 && strcmp(input, "no") != 0 )
	{
		printf("\n\nWould you like to play again? yes or no: ");
		//Get yes or no
		//fgets(userInput, MAX, stdin);
		//get rid of newline on user input
		//userInput[strlen(userInput) - 1] = '\0';
		scanf("%s", input);
	}

	if (strcmp(input, "no") == 0)
	{
		printf("\nToo bad...\n");
	}
	else //else play the game
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //"clears" for a new game
		play();
	}


}




/*
Your job is to write a program where a human can play against the computer in a best-of-5 
tournament. The first to win three games wins the match. Have the human player enter their 
choice as text, and then have the computer randomly pick its choice. If the two match, the game 
is a tie and doesn’t count. Otherwise you will add one to the score of the winner. 
After the match is over, you should ask the user if they would like to play again.




Welcome to Rock, Paper, Scissors

Would you like to play? yes

What is your choice? scissors
The computer chooses rock. You lose this game!

The score is now you: 0 computer: 1
*/