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
int determine_file_action(char *, int, int, int);
int redirect_set(char *);

//Globals:
int a, o, i; //Booleans for appean, redirect_out, and redirect_in


//Usage:
int main(void){
	//Declaration:
	char *user_input = malloc(sizeof(char)*100);
	char *command = malloc(sizeof(char)*100);

	//Usage:
	printf("\n\t\t     Welcome to Shellax\n\tA POSIX Shell by Buck Young (bcy3@pitt.edu)\n\n");//Welcome message

	while(1){
		//Reinit booleans
		a=0;
		o=0;
		i=0;

		printf("shellax-1.0$ "); //user prompt
		if( fgets(user_input, 100, stdin) != NULL ){ //GET LINE FROM USER

			//before tokenizing, check for redirects
			redirect_set(user_input);

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
	char *filename = malloc(sizeof(char)*100);
	char *argv[20];
	int argc = 0;

	//Put command into array[0]
	argv[argc++] = command; //argc now equals 1!

	//Store any parameters (could include a filename!!!)
	token = strtok(NULL, " \t\n()<>|&;"); //get next token 
	while (token != NULL){
		argv[argc++] = token; //store token in parameter list
 		token = strtok(NULL, " \t\n()<>|&;"); //get next token 
 	}

 	if (a || o || i){ //If we need to redirect
 		//get filename from argc-1, then set that to NULL
 		argc--;
 		filename = argv[argc];
 		argv[argc] = NULL;
 		//set stream
 		determine_file_action(filename, a, o, i);
 	} else {
 		argv[argc] = NULL; //Delimit list with NULL at argc
 	}

 	 

 	//Call program and error cases//
 	if ( execvp(command, argv) == -1 ){
		printf("-shellax: %s: command not found\n", command); //Error
	}

	exit(EXIT_SUCCESS); //Safety exit -- shouldnt really happen
}

//Sets a, o, i booleans if a redirect is needed
int redirect_set(char *user_input){
	char *tmp = malloc(sizeof(char)*100);

	//if append 
	tmp = strstr(user_input, ">>"); 
	if (tmp != NULL){
		a = 1; //true
		return 0;
	}

	//if redirect out
	tmp = strstr(user_input, ">"); 
	if (tmp != NULL){
		o = 1; //true
		return 0;
	}

	//if redirect in
	tmp = strstr(user_input, "<"); 
	if (tmp != NULL){
		i = 1; //true
		return 0;
	}

}

//Determines if we should append, out, or in, the input/output stream
int determine_file_action(char* filename, int a, int o, int i){
	if (a){
		append(filename);
		return 0;
	}
	if (o){
		re_out(filename);
		return 0;
	}
	if (i){
		re_in(filename);
		return 0;
	}

}

//append logic -- sets stream to append
//Accepts untokenized filename
void append(char* filename){
	FILE *f;

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

	//Set stream (freopen)
	if (filename != NULL){ //We need to freopen it and set the stream!
		f = freopen(filename, "r", stdin); //FREopen
		if (f == NULL){ //error
			printf("-shellax: %s: error opening file\n", filename);
		}
	}	

}













