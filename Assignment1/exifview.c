#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structs
struct header {
	unsigned short jpg_start_of_file;
	unsigned short jpg_app1_marker;
	unsigned short app1_block_length;
	unsigned char exif_string[4];
	unsigned short null_zero;
	unsigned char endianness[2];
	unsigned short version_42;
	unsigned int exif_block_offset;
};

struct tiff_tag {
	unsigned short tag_id;
	unsigned short data_type;
	unsigned int num;
	unsigned int offset; //(or data)
};

//Prototypes
int verify(struct header*);
void print_data(int, int, FILE*);
void parse_tiffs(FILE*);
void parse_tiffs_sub(FILE*);

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

		//Verified, now parse for tiffs
		parse_tiffs(f);



	} else { //FAILED to read in a header struct
		printf("Failed to read a single header struct from file");
		return -2;
	}

	//CleanUp
	fclose(f);
	return 0;

}

/*
	verify() accepts the address of the header struct and will verify that the header is in good order and verify the endianness
	@returns: 0 if failed, !0 if success
*/
int verify(struct header* s){

		char a[5];

		//Verify this is a jpg
		if ( !(s->jpg_start_of_file == 0xD8FF) ){
			printf("Error: Cannot verify file as JPG\n");
			return 0;
		}

		//Verify this is a APP1
		if ( !(s->jpg_app1_marker == 0xE1FF) ){ 
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

/*
	This method will accept a file and parse it for count tiff tags
	(specifically for the manufacturer and camera model tags)
*/
void parse_tiffs(FILE* f){
	//declarations:
	unsigned short count;
	int i;
	struct tiff_tag tiff;
	
	//Usage:
	//get count
	fread(&count, sizeof(short), 1, f); //at byte 20-21

	//for each count, read in tiff tag
	for (i = 0; i < count; i++){
		//Where are the tags? starting at byte 22. Note: tags are 12 bytes long
		//OFFSET => 22+(12*i)
		fread(&tiff, sizeof(struct tiff_tag), 1, f);

		if (tiff.tag_id == 0x010F){
			//Manufacturer 
			printf("%-16s", "Manufacturer: ");
				print_data(tiff.num, tiff.offset, f); //Print the manufacturer string
			printf("\n");

		} else if (tiff.tag_id == 0x0110){
			//Camera Model
			printf("%-16s", "Model: ");
			print_data(tiff.num, tiff.offset, f); //Print the camera model string
			printf("\n");

		} else if (tiff.tag_id == 0x8769){
			//Exif sub-block address
			//Change the file-pointer, and dont worry about changing it back
			fseek(f, (tiff.offset+12), SEEK_SET);
			parse_tiffs_sub(f);
			break; //After we encounter this tag (8769), we can stop searching!
	
		}
	}
}

void parse_tiffs_sub(FILE* f){
	//declarations:
	unsigned short count;
	int i;
	struct tiff_tag tiff;
	
	//Usage:
	//get count
	fread(&count, sizeof(short), 1, f); //at byte 20-21

	//for each count, read in tiff tag
	for (i = 0; i < count; i++){
		//Where are the tags? starting at byte 22. Note: tags are 12 bytes long
		//OFFSET => 22+(12*i)
		fread(&tiff, sizeof(struct tiff_tag), 1, f);

		//IF TAG BLOCK
		
	}
}

/*
	print_m_m() prints n characters from offset (o+12) in file f
	it responsibly and intelligently controls the file-pointer (that is, it retains position across a call to print_m_m)
*/
void print_data(int n, int o, FILE* f){

	long long int fpointer = ftell(f); //Saves file-pointer
	char* a = malloc(n * sizeof(char));//create space for n characters of size 1byte (yes, redundant)

	if (a == NULL){
		printf("\nERROR ALLOCATING MEMORY IN print_m_m()\n");
	}

	o = o+12; //create actual address

	fseek(f, o, SEEK_SET); //go to location in file

	fread(a, sizeof(char), n, f); //read in n chars from file f: place into a (note: n includes a string-sentinel value)
	printf("%s", a);


	fseek(f, fpointer, SEEK_SET); //Returns file-pointer
	free(a); //Frees memory
}
