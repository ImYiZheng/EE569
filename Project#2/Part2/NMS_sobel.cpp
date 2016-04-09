//
//  main.cpp
//  Sobel_edge_detection improved by non_maximun_suppression
//
//  created by Yi Zheng on 10/01/15.
//	version 1.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int bytesPerPixel;
	// set the image size
	int size1 = 481;
	int size2 = 321;
	int N=3;
	float threshold = 0.15; //set the thresholds
	int tmp1[size2][size1+N-1];
	int tmp2[size2+N-1][size1+N-1];

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
			//size = atoi(argv[4]);
		}
	}

	// Allocate image data array
	unsigned char imagedata[size2][size1][bytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(imagedata, sizeof(unsigned char), size2*size1*bytesPerPixel, file);
	fclose(file);

	int imagedata_gray[size2][size1];
	unsigned char imagedata_out[size2][size1];
	double image_gradient[size2][size1];
	int i,j,k,l;
	//convert the RGB to grayscale image
	for(i=0;i<size2;i++) {
		for(j=0;j<size1;j++) {
			imagedata_gray[i][j]=((int)imagedata[i][j][0]*38+(int)imagedata[i][j][1]*75+(int)imagedata[i][j][2]*15)>>7;
		}
	}
	//matrix extension
	for(i=0;i<=size2-1;i++) {
		for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++) {
			tmp1[i][j] = imagedata_gray[i][j-(N-1)/2];
		}
	}
	for(i=0;i<=size2-1;i++) {
		for(j=0;j<(N-1)/2;j++) {
			tmp1[i][j] = imagedata_gray[i][(N-1)/2-j];
			tmp1[i][size1+(N-1)/2+j] = imagedata_gray[i][size1-((N-1)/2+j)];
		}
	}
	for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++) {
		for(j=0;j<=size1+(N-1)-1;j++) {
			tmp2[i][j] = tmp1[i-(N-1)/2][j];
		}
	}
	for(j=0;j<=size1+(N-1)-1;j++) {
		for(i=0;i<(N-1)/2;i++) {
			tmp2[i][j] = tmp1[(N-1)/2-i][j];
			tmp2[size2+(N-1)/2+i][j] = tmp1[size2-((N-1)/2+i)][j];
		}
	}
	//get the gradient magnitude
	float theta[size2][size1]; // define the direction of the magnitude
	float dx,dy; // define the derivative for x and y direction
	double mag;
	for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++) {
		for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++) {

			dx = (tmp2[i-1][j+1]+2*tmp2[i][j+1]+tmp2[i+1][j+1]-tmp2[i-1][j-1]-2*tmp2[i][j-1]-tmp2[i+1][j-1])/4;
			dy = (tmp2[i-1][j-1]+2*tmp2[i-1][j]+tmp2[i-1][j+1]-tmp2[i+1][j-1]-2*tmp2[i+1][j]-tmp2[i+1][j+1])/4;
			mag = sqrt(dx*dx+dy*dy);
			image_gradient[i-(N-1)/2][j-(N-1)/2]=mag;
			theta[i-(N-1)/2][j-(N-1)/2]=(dy)/(dx);
		}
	}

	//normalization of the gradient magnitude
	double max = image_gradient[0][0],min =image_gradient[0][0];
	for(i=0;i<size2;i++) {
		for(j=0;j<size1;j++) {
			if (image_gradient[i][j]>max) {
				max = image_gradient[i][j];
			}
			if (image_gradient[i][j]<min) {
				min = image_gradient[i][j];
			}
		}
	}
	for(i=0;i<size2;i++) {
		for (j=0;j<size1;j++){
			imagedata_gray[i][j]=(int)(255*(image_gradient[i][j]-min)/(max-min));
		}
	}
	// rearrange the pixel according to the gradient magnitude
	int height[size2*size1],width[size2*size1];
	int boundary;
	l=0;
	for(k=0;k<256;k++){
		for(i=0;i<size2;i++) {
			for(j=0;j<size1;j++) {
			if(imagedata_gray[i][j]==k)
			{
				height[l]=i;
				width[l]=j;
				l++;
			}
			}
			}
	}
	boundary = (int)((1-threshold)*size2*size1);
	for(i=0;i<boundary;i++)
	{
		imagedata_out[height[i]][width[i]] = (unsigned char)255;
	}
	for(i=boundary;i<size2*size1;i++) // for the edge points, using NMS to determine whether it is a edge point
	{ if(theta[height[i]][width[i]]<1) {
			if (imagedata_gray[height[i]][width[i]] > imagedata_gray[height[i]][width[i] - 1] && imagedata_gray[height[i]][width[i]] > imagedata_gray[height[i]][width[i] + 1]) {
				imagedata_out[height[i]][width[i]] = (unsigned char) 0;
			}
			else {
				imagedata_out[height[i]][width[i]] = (unsigned char) 255;
			}
		}
		else{
			if (imagedata_gray[height[i]][width[i]] > imagedata_gray[height[i] - 1][width[i]] && imagedata_gray[height[i]][width[i]] > imagedata_gray[height[i] + 1][width[i]]) {
				imagedata_out[height[i]][width[i]] = (unsigned char) 0;
			}
			else {
				imagedata_out[height[i]][width[i]] = (unsigned char) 255;
			}
		}
	}
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(imagedata_out, sizeof(unsigned char), size2*size1*1, file);
	fclose(file);
	return 0;
}