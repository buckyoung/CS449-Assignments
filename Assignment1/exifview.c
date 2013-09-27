#include <stdio.h>

//Prototypes

//Declarations

//Structs
struct header {
	char jpg_start_of_file[2];
	char jpg_app1_marker[2];
	char app1_block_length[2];
	char exif_string[4];
	char null_zero[2];
	char endianess[2];
	char version_42[2];
	char exif_block_offset[4];
};

struct tiff_tag {
	char tag_id[2];
	char data_type[2];
	char num_of_data[4];
	char value_or_offset[4];
};

int main(int argc, char argv[]){

	//Check for a single command-line argument
	if (argc != 2){
		printf("Usage: ./exifview filename.jpg\n");
		return -1;
	}

	//Declarations
	FILE* f;
	struct header exif_head;

	//Usage
	f = fopen(argv[1], "rb"); //open file to read binary

	if (fread(&exif_head, sizeof(struct header), 1, f) == 1){ //Successfully read in exif/tiff header struct
		printf("\nDebug| FF: %X | D8: %X | FF: %X | E1: %X \n", exif_head.jpg_start_of_file[0], exif_head.jpg_start_of_file[1], exif_head.jpg_app1_marker[0], exif_head.jpg_app1_marker[1]); //DEBUG


	} else { //FAILED to read in a header struct
		printf("Failed to read a single header struct from file");
		return -2;
	}


}