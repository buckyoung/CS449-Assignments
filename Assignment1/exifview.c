#include <stdio.h>
#include <string.h>

//Structs
struct header {
	unsigned char jpg_start_of_file[2];
	unsigned char jpg_app1_marker[2];
	unsigned char app1_block_length[2];
	unsigned char exif_string[4];
	unsigned char null_zero[2];
	unsigned char endianness[2];
	unsigned char version_42[2];
	unsigned char exif_block_offset[4];
};

struct tiff_tag {
	char tag_id[2];
	char data_type[2];
	char num_of_data[4];
	char value_or_offset[4];
};

//Prototypes
int verify(struct header*);

//Declarations

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
	f = fopen("img1.jpg", "rb"); //open file to read binary //TODO (SegFault on argv[1]?!)


	if (fread(&exif_head, sizeof(struct header), 1, f) == 1){ //Successfully read in exif/tiff header struct

		//Verify JPG and APP1
		if (!verify(&exif_head)){ //returns 0 if failed
			printf("Exiting...\n");
			return -3;
		} 

		//Verified file, now lets start reading the TIFFs


	} else { //FAILED to read in a header struct
		printf("Failed to read a single header struct from file");
		return -2;
	}


}

/*
	verify() accepts the address of the header struct and will verify that the header is in good order and verify the endianness
	@returns: 0 if failed, !0 if success
*/
int verify(struct header* s){

		char a[5];

		//Verify this is a jpg
		if ( !(s->jpg_start_of_file[0] == 0xFF && s->jpg_start_of_file[1] == 0xD8) ){
			printf("Error: Cannot verify file as JPG\n");
			return 0;
		}

		//Verify this is a APP1
		if ( !(s->jpg_app1_marker[0] == 0xFF && s->jpg_app1_marker[1] == 0xE1) ){ 
			printf("Error: Cannot verify file as APP1\n");
			return 0;
		}

		//Verify EXIF string
		strcpy(a, s->exif_string); //copy "EXIF" into a
		a[4] = '\0'; //add null terminator 
		if ( !strcmp(a, "EXIF") ){
			printf("Error: Tag not found\n");
			return 0;
		}

		//Verify Endianness
		if ( !(s->endianness[0] == 'I' && s->endianness[1] == 'I')){
			printf("Error: Endianness not supported");
			return 0;
		}

		//else, everything is good!
		return 1;

}