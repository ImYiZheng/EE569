//
//  PSNR.cpp
//  calculate the PSNR of am image
//
//  created by Yi Zheng on 9/9/15.
//  version 3.0
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int bytesPerPixel;
	int size = 512;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		bytesPerPixel = 1; // default is grey image
	}
	else {
		bytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			size = atoi(argv[4]);
		}
	}

	// Allocate image data array
	unsigned char Imagedata[size][size][bytesPerPixel];
	unsigned char Imagedata_new[size][size][bytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata_new, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int i,j,l,val;
	double mse,psnr;
	mse=0;
	for(l=0;l<3;l++)
	{

		for(i=0;i<size;i++)
		{
			for(j=0;j<size;j++)
			{
				val=abs((int)Imagedata_new[i][j][l]-(int)Imagedata[i][j][l]);
				mse=mse+val*val;
			}
		}

	}
	mse=mse/(size*size*3);
	psnr=10*log10(255*255/mse);
	cout<<psnr<<endl;
	return 0;
}
