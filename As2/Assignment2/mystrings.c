/*
 *  This program accepts a filename as a CMDline argument and will print out any readable "strings" in the file's binary.
 *  Buck Young 2013.10.13
 */
#include <stdio.h>

//prototypes
 int valid_char(char);

int main(int argc, char* argv[]){

	//Declarations:
	FILE *f;
	char four_bytes[4];
	long position_indicator_location;
	short read_return;
	char c;


	//Usage:
	if (argc != 2){
	    printf("Usage: ./mystrings file_name\n");
	    return 1;
	}

	f = fopen(argv[1], "rb");

	do{
	//get seeker location
	position_indicator_location = ftell(f);
	//get 4 bytes
	read_return = fread(&four_bytes, 1, 4, f);
	//check return of fread
	if (read_return != 4){
		printf("\n\n[-EOF-]\n\n");
		return 3;
	} else {
	//for each byte of return quantity
	//examine four and determine if between 32 and 126, inclusive
		if (valid_char(four_bytes[0]) && valid_char(four_bytes[1]) && valid_char(four_bytes[2]) && valid_char(four_bytes[3])){
			//Start reading from position_indicator_location +4 to get the rest of the string
			fseek(f, position_indicator_location, SEEK_SET);
			while (!feof(f) && read_return > 0){
				read_return = fread(&c, 1, 1, f); //read in a char
				if (valid_char(c)){ //if printable, print it
					printf("%c", c);
				} else { //break out!
					printf("\n"); //set new line.
					break;
				}
			}
		} else { //not valid chars
			//only advance one -- (-3)
			fseek(f, ftell(f)-3, SEEK_SET);
		}

	}
	} while(1); //hm.
	
}

int valid_char(char c){

	//valid chars
	if (c >= 32 && c <= 126){
		return 1; //TRUE
	}

	//ELSE
	return 0;//false
}