//I affirm that I wrote this program myself without any help from any other people or sources from the internet.
//JUSTIN GONZALEZ
//6155658
//This program uses functions provided in the bmplib.h as well as the fliphorizontal function used in main.c in order to edit bmp images
//These commands can be chained. Getopt is also used not only to perform modifications to the image but also to select a specific input file and output file.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "bmplib.h"

void error(){
printf("bmptool [-f | -r | -l | -s scale| -v ] [-o output_file] [input_file]");
}

int fliphorizontal(PIXEL *original, PIXEL **new, int rows, int cols){
int row;
int col;

if((rows <= 0) || (cols <= 0)){
return -1;
}

*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

//FLIPS COLOUMNS TO FLIP THE IMAGE
for(row = 0; row < rows; row++){
	for(col = 0; col < cols; col++){
	PIXEL* o = original + row*cols + col;
	PIXEL* n = (*new) + row*cols + (cols-1-col);
	*n = *o;
	}
}

return 0;
}


int flipvertical(PIXEL *original, PIXEL **new, int rows, int cols){
int row;
int col;

if((rows <= 0) || (cols <= 0)){
return -1;
} 

*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

//FLIPS ROWS TO MAKE THE IMAGE UPSIDEDOWN
for(row = 0; row < rows; row++){
	for(col = 0; col < cols; col++){
        PIXEL* o = original + row*cols + col;
        PIXEL* n = (*new) + (rows - 1 - row) * cols + col;
        *n = *o;
        }
}


return 0;
}


int rotateclockwise(PIXEL* original, int rows, int cols, PIXEL** new){
int row;
int col;

*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

if((rows <= 0) || (cols <= 0)){
return -1;
}

//ROTATES THE FULL WAY AROUND TO SHOW COUNTER CLOCKWISE ROTATION
for(row = 0; row < rows; row++){
	for(col = 0; col < cols; col++){
	PIXEL* o = original + (row*cols) + col;
	PIXEL* n = (*new) + ((cols - col - 1)*rows) + row;
	*n = *o;
	}

}

return 0;
}


int rotatecounter(PIXEL* original, int rows, int cols, PIXEL** new){
int row;
int col;

*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

//ROTATES COLOUMNS AND ROWS
for(row = 0; row < rows; row++){
	for(col = 0; col < cols; col++){
	PIXEL* o = original + row*cols + col;
	PIXEL* n = (*new) + (rows-(row+1)) + (col*cols);
	*n = *o;
	}
}
return 0;
}

int scale(PIXEL* original, int rows, int cols, int factor, PIXEL** new, int* newrows, int* newcols){
int row;
int col;
int j;
int i;

if((rows <= 0) || (cols <= 0)){
return -1;
}

*newrows = rows * factor;
*newcols = cols * factor;

*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));

//DUPLICATES ROWS AND COLOUMNS AND MOVES THEM TO ENARLGE THE IMAGE
for(row = 0; row < rows; row++){
	for(col = 0; col < cols; col++){
	PIXEL* o = original + row*cols + col;
		for(j = 0; j < factor; j++){
			for(i = 0; i < factor; i++){
			PIXEL* n = (*new) + (row * factor + i) * (*newcols) + (col * factor + j);
			*n = *o;
			}
		}
	}
}
return 0;

}

int main(int argc, char **argv){

//IMAGE MANIPULATION OR FILENAME VARIABLES
int option;
char outputfilename[15];
char inputfilename[15];
int factor;
int newrow;
int newcol;
int row;
int col;

//FLAGS
int scalecount = 0;
int rotatecount = 0;
int countercount = 0;
int flipcount = 0;
int fliphorizontalcount = 0;
int outputcount = 0;

PIXEL *b, *nb;

while((option = getopt(argc, argv, "frls:vo:")) != -1){

	switch(option){
	case 's': //SCALES IMAGE BY A FACTOR OF 2 OR 3
	scalecount++;
	factor = atoi(optarg);
	break;

	case 'r': //ROTATE 90 CLOCKWISE
	rotatecount++;
	break;

	case 'l': //ROTATE 90 COUNTERCLOCKWISE
	countercount++;
	break;

	case 'v'://FLIPS VERTICALLY
	flipcount++;
	break;

	case 'f'://FLIPS IMAGE HORIZONTALLY
	fliphorizontalcount++;
	break;

	case 'o'://FLIPS IMAGE HORIZONTALLY
	outputcount++;
	//STORES OUTPUT FILENAME
	strncpy(outputfilename, optarg, 15);
	break;

	default:
	error();
	break;
}
}

//catches if no arguements were passed
if(scalecount == 0 && rotatecount == 0 && countercount == 0 && flipcount == 0 && fliphorizontalcount == 0 && outputcount == 0){
printf("Must enter at least one arguement\n");
error();
}

//catches if no arguments were passed besides the outputfile
if(scalecount == 0 && rotatecount == 0 && countercount == 0 && flipcount == 0 && fliphorizontalcount == 0 && outputcount == 1 ){
printf("At least one other arguement mandatory when passing  -o\n");
error();
}

//NO INPUT FILE GIVEN FOR CHAINS
if(optind == argc){
readFile(NULL, &row, &col, &b);
}else{//INPUTFILE GIVEN
strcpy(inputfilename, argv[optind]);
printf("Reading from inputfile: %s\n", inputfilename);
readFile(inputfilename, &row, &col, &b);
}

//SCALE
if(scalecount == 1){
	if(factor != 2 && factor != 3){
	printf("Please enter a value of 2 or 3\n");
	error();
	}else{

	printf("Image was scaled by a factor of %d\n", factor);
	scale(b, row, col, factor, &nb, &newrow, &newcol);
//USED TO ENLARGE IMAGE	
	row = newrow;
	col = newcol;
	}
}
//ROTATE R
if(rotatecount == 1){
printf("Image was rotated clockwise\n");
	
	if(scalecount == 1){//USED TO APPLY MULTIPLE TRANSFORMATIONS IN ORDER
	rotateclockwise(nb, row, col, &nb);
	}else{
	rotateclockwise(b, row, col, &nb);//SIMPLY DOES ONE TRANSFORMATION
	}

}
//COUNTERCLOCKWISE L
if(countercount == 1){
printf("Image was rotated counterclockwise\n");
	
	if(scalecount == 1 || rotatecount == 1){//USED TO APPLY MULTIPLE TRANSFORMATIONS IN ORDER
	rotatecounter(nb, row, col, &nb);
	}else{
	rotatecounter(b, row, col, &nb);//SIMPLY DOES ONE TRANSFORMATION
	}

}

//FLIP VERTICALLY V
if(flipcount == 1){
printf("Image flipped vertically\n");

	if(scalecount == 1 || rotatecount == 1 || countercount == 1){//USED TO APPLY MULTIPLE TRANSFORMATIONS IN ORDER
	flipvertical(nb, &nb, row, col);
	}else{
	flipvertical(b, &nb, row, col);//SIMPLY DOES ONE TRANSFORMATION
	} 
}

//FLIP HORIZONTALLY F
if(fliphorizontalcount == 1){
	printf("Image was flipped horizontally\n");

	if(scalecount == 1 || rotatecount == 1 || flipcount == 1 || countercount == 1){//USED TO APPLY MULTIPLE TRANSFORMATIONS IN ORDER
	fliphorizontal(nb, &nb, row, col);
	}else{
	fliphorizontal(b, &nb, row, col); //SIMPLY DOES ONE TRANSFORMATION
	}

}

//STANDARD OUTPUT OF FILE
if(outputcount == 0){
writeFile(NULL, row, col, nb);
}else if(outputcount == 1){//CREATES AN OUTPUTFILE
writeFile(outputfilename, row, col, nb);
printf("File named %s was created\n", outputfilename);
}

//FREES MEMORY
free(b);
free(nb);

return 0;
}
