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
	unsigned short tag_id;
	unsigned short data_type;
	unsigned int num;
	unsigned int offset;
};

//Prototypes
int verify(struct header*);
void print_m_m(int, int, FILE*);

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
	unsigned short count;
	int i;
	struct tiff_tag tiff;


	//Usage
	f = fopen("img1.jpg", "rb"); //open file to read binary //TODO (SegFault on argv[1]?!)


	if (fread(&exif_head, sizeof(struct header), 1, f) == 1){ //Successfully read in exif/tiff header struct

		//Verify JPG and APP1
		if (!verify(&exif_head)){ //returns 0 if failed
			printf("Exiting...\n");
			return -3;
		}

		//Verified file, now get count:
		fread(&count, sizeof(short), 1, f); //at byte 20-21

		//for each count, read in tiff tag
		for (i = 0; i < count; i++){
			//Where are the tags? starting at byte 22. Note: tags are 12 bytes long
			//OFFSET => 22+(12*i)

			fread(&tiff, sizeof(struct tiff_tag), 1, f);

			if (tiff.tag_id == 0x010F){
				//Manufacturer 
				printf("%-16s", "Manufacturer: ");
				print_m_m(tiff.num, tiff.offset, f); //Print the manufacturer string
				printf("\n");

			} else if (tiff.tag_id == 0x0110){
				//Camera Model
				printf("%-16s", "Model: ");
				print_m_m(tiff.num, tiff.offset, f); //Print the camera model string
				printf("\n");

			} else if (tiff.tag_id == 0x8769){
				//Exif sub-block address

			}

		}

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

/*
	print_m_m() prints n characters from offset (o+12) in file f
	it responsibly and intelligently controls the file-pointer (that is, it retains position across a call to print_m_m)
*/
void print_m_m(int n, int o, FILE* f){

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