#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//Prototypes:
void switch_command(char *);
void cd(void);
void posix_program(char *);
void child(char *);
void append(char *);
void re_out(char *);
void re_in(char *);


//Functions:
int main(void){
	//Declaration:
	char *user_input = malloc(sizeof(char)*100);
	char *command = malloc(sizeof(char)*100);

	//Usage:
	printf("\n\t\t     Welcome to Shellax\n\tA POSIX Shell by Buck Young (bcy3@pitt.edu)\n\n");//Welcome message

	while(1){
		printf("shellax-1.0$ "); //user prompt
		if( fgets(user_input, 100, stdin) != NULL ){ //GET LINE FROM USER
			command = strtok(user_input, " \t\n()<>|&;"); //get first token (will be command)
			switch_command(command); //Determine what command it is!
		}
	}

	return 0;

}

void switch_command(char *command){

	if( strcmp(command, "exit") == 0 ){ // EXIT
		printf("\n\t\t Thank you for using Shellax\n\t\t\t (c) 2013\n\n");
		exit(EXIT_SUCCESS);

	} else if ( strcmp(command, "hello") == 0 ){ //for fun
		printf("Why hello there, user.\n");

	} else if ( strcmp(command, "cd") == 0 ){ //CD : accepts relative and absolute
		cd();

	} else { //UNIX COMMAND *in PATH*
		posix_program(command);

	} //end ELSE

}

//----  COMMANDS  ----//

//Change dir
void cd(void){
	char *token = malloc(sizeof(char)*100);
		token = strtok(NULL, " \t\n()<>|&;"); //get next token (will be pathname)

		//Error cases//
		if ( chdir(token) != 0 ){
			if (errno == 2){
				printf("-shellax: cd: %s: No such file or directory\n", token); //Error 
			} else if (errno == 20){
				printf("-shellax: cd: %s: Not a directory\n", token); //Error 
		} else {
				printf("-shellax: cd: %s: error #%d\n", token, errno); //generic error
			}
		}
}

//Run any program
void posix_program(char *command){
	
	pid_t pid;
	pid = fork();

	if(pid == -1){//error
		printf("-shellax: fork: error #\n", errno);

	} else if (pid > 0) {//parent
		waitpid(pid, NULL, 0); //waits for child!

	} else { //CHILD! 
		child(command);
	}
}

//child process logic
void child(char *command){
	//Init:
	char *token = malloc(sizeof(char)*100);
	char *tmp = malloc(sizeof(char)*100);
	char *argv[10];
	int argc = 0;

	//init
	tmp = NULL;
	//Put command into array[0]
	argv[argc++] = command; //argc now equals 1!

	//Get any parameters:
	token = strtok(NULL, " \t\n()|&;"); //get next token 
	while (token != NULL){

		argv[argc++] = token;
 		token = strtok(NULL, " \t\n()|&;"); //get next token 

 		if (token != NULL){
	 		//check for redirects!
	 		tmp = strstr(token, ">>"); //if append is in there, tmp will contain ">> ", else NULL
			if (tmp != NULL){
				tmp = strtok(NULL, " \t\n()|&;"); //get next token (will be filename)
				append(tmp);
				break;
			}
			tmp = strstr(token, ">"); //if redirect out
			if (tmp != NULL){
				tmp = strtok(NULL, " \t\n()|&;"); //get next token (will be filename)
				re_out(tmp);
				break;
			}
			tmp = strstr(token, "<"); //if redirect in
			if (tmp != NULL){
				tmp = strtok(NULL, " \t\n()|&;"); //get next token (will be filename)
				re_in(tmp);
				break;
			}
		}

 	}
 	argv[argc] = NULL; //Delimit list with NULL 

 	//Call program and error cases//
 	if ( execvp(command, argv) == -1 ){
		printf("-shellax: %s: command not found\n", command); //Error
	}

	exit(EXIT_SUCCESS); //Safety exit -- shouldnt really happen
}

//append logic -- sets stream to append
//Accepts untokenized filename
void append(char* filename){
	FILE *f;

	filename = strtok(filename, " \t\n"); //clean up token (will be filename)

	//Set stream (freopen)
	if (filename != NULL){ //We need to freopen it and set the stream!
		f = freopen(filename, "a", stdout); //FREopen
		if (f == NULL){ //error
			printf("-shellax: %s: error opening file\n", filename);
		}
	} 

}

//redirect out logic -- sets stream to out
//Accepts untokenized filename
void re_out(char* filename){
	FILE *f;

	filename = strtok(filename, " \t\n"); //clean up token (will be filename)

	//Set stream (freopen)
	if (filename != NULL){ //We need to freopen it and set the stream!
		f = freopen(filename, "w", stdout); //FREopen
		if (f == NULL){ //error
			printf("-shellax: %s: error opening file\n", filename);
		}
	}	

}

//redirect in logic -- sets stream to in
//Accepts untokenized filename
void re_in(char* filename){
	FILE *f;

	filename = strtok(filename, " \t\n"); //clean up token (will be filename)

	//Set stream (freopen)
	if (filename != NULL){ //We need to freopen it and set the stream!
		f = freopen(filename, "r", stdin); //FREopen
		if (f == NULL){ //error
			printf("-shellax: %s: error opening file\n", filename);
		}
	}	

}













