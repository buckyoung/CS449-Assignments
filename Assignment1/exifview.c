#include <stdio.h>

//Prototypes

//Declarations

//Structs
struct header {
	unsigned char jpg_start_of_file[2];
	unsigned char jpg_app1_marker[2];
	unsigned char app1_block_length[2];
	unsigned char exif_string[4];
	unsigned char null_zero[2];
	unsigned char endianess[2];
	unsigned char version_42[2];
	unsigned char exif_block_offset[4];
};

struct tiff_tag {
	char tag_id[2];
	char data_type[2];
	char num_of_data[4];
	char value_or_offset[4];
};

int main(int argc, char argv[]){

	//Declarations
	FILE* f;
	struct header exif_head;

	//Check for a single command-line argument
	if (argc != 2){
		printf("Usage: ./exifview filename.jpg\n");
		return -1;
	}

	//Usage
	
	f = fopen("img1.jpg", "rb"); //open file to read binary


	if (fread(&exif_head, sizeof(struct header), 1, f) == 1){ //Successfully read in exif/tiff header struct

		//Verify JPG and APP1
		if ( !(exif_head.jpg_start_of_file[0] == 0xFF && exif_head.jpg_start_of_file[1] == 0xD8) ){ //Verify this is a jpg
			printf("Error: Cannot verify file as JPG");
			return -3;
		}
		if ( !(exif_head.jpg_app1_marker[0] == 0xFF && exif_head.jpg_app1_marker[1] == 0xE1) ){ //Verify this is a APP1
			printf("Error: Cannot verify file as APP1");
			return -4;
		}


	} else { //FAILED to read in a header struct
		printf("Failed to read a single header struct from file");
		return -2;
	}


}