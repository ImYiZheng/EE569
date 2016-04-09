//
//  main.cpp
//  image resizing via Bilinear Interpolation
//
//  created by Yi Zheng on 9/7/15.
//	version 1.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	int size_new = 650;		//new image size
	int width,height,channel;
	int x,y;
	int tmp;
	float a,b;
	unsigned char Imagedata_new[size_new][size_new][bytesPerPixel];
	// Image resizing by bilinear interpolation
	for(channel=0;channel<3;channel++)
	{
		for(height=0;height<650;height++)
		{
			for(width=0;width<650;width++)
			{
				x = height*(size-1)/(size_new-1);
				y = width*(size-1)/(size_new-1);
				a = (float)height*(size-1)/(size_new-1) - x;
				b = (float)width*(size-1)/(size_new-1) - y;
				if(x==511&&y==511)
				{
					Imagedata_new[649][649][channel] = Imagedata[511][511][channel];
					continue;
				}
				if(x==511&&y!=511)
				{
					a = 1;
					tmp = (1-a)*(1-b)*(int)Imagedata[x-1][y][channel]+b*(1-a)*(int)Imagedata[x-1][y+1][channel]+a*(1-b)*(int)Imagedata[x][y][channel]+a*b*(int)Imagedata[x][y+1][channel];
					Imagedata_new[height][width][channel] = (char)tmp;
					continue;
				}
				if(y==511&&x!=511)
				{
					b = 1;
					tmp = (1-a)*(1-b)*(int)Imagedata[x][y-1][channel]+b*(1-a)*(int)Imagedata[x][y][channel]+a*(1-b)*(int)Imagedata[x+1][y-1][channel]+a*b*(int)Imagedata[x+1][y][channel];
					Imagedata_new[height][width][channel] = (char)tmp;
					continue;
				}
				tmp = (1-a)*(1-b)*(int)Imagedata[x][y][channel]+b*(1-a)*(int)Imagedata[x][y+1][channel]+a*(1-b)*(int)Imagedata[x+1][y][channel]+a*b*(int)Imagedata[x+1][y+1][channel];
				Imagedata_new[height][width][channel] = (char)tmp;
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_new, sizeof(unsigned char), size_new*size_new*bytesPerPixel, file);
	fclose(file);
	return 0;
}