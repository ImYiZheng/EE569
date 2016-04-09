//
//  main.cpp
//  scalar color half-toning
//
//  created by Yi Zheng on 10/22/15.
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
    int bytesPerPixel = 3;
    int size = 512;

    // Allocate image data array
    unsigned char imagedata[size][size][bytesPerPixel];
    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(imagedata, sizeof(unsigned char), size*size*bytesPerPixel, file);
    fclose(file);

    unsigned char imagedata_out[size][size][bytesPerPixel];
    double tmp[size][size][bytesPerPixel];
    double error;
    // normalize and transform the image from the display-based to printing-based
    int i,j,l;
    for (l=0;l<3;l++){
        for (i=0;i<size;i++) {
            for (j=0;j<size;j++){
                tmp[i][j][l] = 1-(double)imagedata[i][j][l]/255;
            }
        }
    }
    // error diffusion for three CMY channels respectively
    for (l=0;l<3;l++) {
        for (i=0;i<size;i++) {
            for (j=0;j<size;j++){
                // compare with the threshold and get the error
                if (tmp[i][j][l]>=0.5) {
                    error = tmp[i][j][l]-1;
                    imagedata_out[i][j][l] = (unsigned char) 0; // over the threshold, set the pixel value black
                }
                else {
                    error = tmp[i][j][l]-0;
                    imagedata_out[i][j][l] = (unsigned char) 255; // below the threshold, set the pixel value white
                }
                // error diffusion using Floyd-Steinberg’s matrix
                if (j+1<size){tmp[i][j+1][l]=tmp[i][j+1][l]+error*7/16;}
                if (j-1>=0&&i+1<size){tmp[i+1][j-1][l]=tmp[i+1][j-1][l]+error*3/16;}
                if (i+1<size){tmp[i+1][j][l]=tmp[i+1][j][l]+error*5/16;}
                if (i+1<size&&j+1<size){tmp[i+1][j+1][l]=tmp[i+1][j+1][l]+error*1/16;}
            }
        }
    }
    // output the image
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_out, sizeof(unsigned char), size*size*bytesPerPixel, file);
    fclose(file);
    return 0;
}