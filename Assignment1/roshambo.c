#include <stdio.h>


int main(void)
{
	char* userInput;

	//Welcome
	printf("Welcome to Rock, Paper, Scissors \n");
	
	while (userInput != "yes" && userInput != "no"){
		printf("Would you like to play? yes or no: ");
		//Get yes or no
		scanf(userInput);
	}

	//If the user wants to stop
	if (userInput == "no"){
		printf("\nToo bad...\n");
		return 0;
	}
	//else, continue

	//Get user choice
	while (userInput != ("rock" || "paper" || "scissors")){
		printf("\nWhat is your choice? rock paper or scissors: ");
		scanf(userInput);
	}

	//Computers random choice
	



	//Would you like to play?
	//What is your choice?
	//The computer chooses rock
	//Determine winner
	//Display score

}


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

…