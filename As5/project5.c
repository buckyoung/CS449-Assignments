#include <unistd.h>

char *user_input = malloc(100); //


int main(void){
	printf("Welcome to Shellax - A POSIX Shell by Buck Young (bcy3@pitt.edu)\n");

	while(1){
		printf("shellax-1.0$ ");
		if( fgets(user_input, 100, stdin) != NULL ){

			 			token = strtok(user_input, " "); //get first token
 			while (token != NULL){
 				reroll_input[i++] = atoi(token); //string to int convert
 				token = strtok(NULL, " "); //get next token
 			}

		}

	}

}