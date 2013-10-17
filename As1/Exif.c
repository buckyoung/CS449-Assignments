                                   

#include <stdio.h>


int main(){

FILE *f;
short count;
int y;

f = fopen("img1.jpg","rb");
char c [16000];
fgets(c,16000,f);
/*
count = c[20]; //c20 and c21
count--; //last index
for ( ; count >=0; count--){

   short* tag_id = &c[22]+(count*12); //address of c22 plus offset

if( *tag_id == 271 ){
        short data_type = *(tag_id+2);
                print("HOLY FUCK WHAT AM I DOING: %h\n\n\n\n", data_type);

   }

}
*/
                              
printf("TEST: %c\n\n\n\n", c[6]);
printf("short: %hu",c[20]);
for(y = 0;y<16000;y++){
printf("%d: %x\n",y, c[y] & 0xff );
}
//printf("%x\n",c[1]);

//printf("%x\n", c[2]);
//printf("%x\n", c[3]);


return 0;
}

