//
//  main.cpp
//  error diffusion using three diffusion matrices
//
//  created by Yi Zheng on 10/20/15.
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
    int size = 512;

    // Allocate image data array
    unsigned char imagedata[size][size];
    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(imagedata, sizeof(unsigned char), size*size, file);
    fclose(file);
    // define the three output images using three diffusion matrices
    unsigned char imagedata_out[size][size];
    unsigned char imagedata_out2[size][size];
    unsigned char imagedata_out3[size][size];
    double tmp[size][size];
    double tmp2[size][size];
    double tmp3[size][size];
    double error;
    // Bayer index matrices
    int i,j;
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++){
            // normalize and transform the input image from display-based to printing-based
            tmp[i][j] = 1-(double)imagedata[i][j]/255;
            tmp2[i][j] = 1-(double)imagedata[i][j]/255;
            tmp3[i][j] = 1-(double)imagedata[i][j]/255;
        }
    }
    // Floyd-Steinberg’s matrix
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++){
            // compare the threshold and get the error
            if (tmp[i][j]>=0.5) {
                error = tmp[i][j]-1;
                imagedata_out[i][j] = (unsigned char) 0;
            }
            else {
                error = tmp[i][j]-0;
                imagedata_out[i][j] = (unsigned char) 255;
            }
            // distribute the error
            if (j+1<size){tmp[i][j+1]=tmp[i][j+1]+error*7/16;}
            if (j-1>=0&&i+1<size){tmp[i+1][j-1]=tmp[i+1][j-1]+error*3/16;}
            if (i+1<size){tmp[i+1][j]=tmp[i+1][j]+error*5/16;}
            if (i+1<size&&j+1<size){tmp[i+1][j+1]=tmp[i+1][j+1]+error*1/16;}
        }
    }
    // JJN matrix
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++){
            // compare the threshold and get the error
            if (tmp2[i][j]>=0.5) {
                error = tmp2[i][j]-1;
                imagedata_out2[i][j] = (unsigned char) 0;
            }
            else {
                error = tmp2[i][j]-0;
                imagedata_out2[i][j] = (unsigned char) 255;
            }
            // distribute the error
            if (j+1<size){tmp2[i][j+1]=tmp2[i][j+1]+error*7/48;}
            if (j+1<size&&i+1<size){tmp2[i+1][j+1]=tmp2[i+1][j+1]+error*5/48;}
            if (j+1<size&&i+2<size){tmp2[i+2][j+1]=tmp2[i+2][j+1]+error*3/48;}
            if (j+2<size){tmp2[i][j+2]=tmp2[i][j+2]+error*5/48;}
            if (j+2<size&&i+1<size){tmp2[i+1][j+2]=tmp2[i+1][j+2]+error*3/48;}
            if (j+2<size&&i+2<size){tmp2[i+2][j+2]=tmp2[i+2][j+2]+error*1/48;}
            if (i+1<size){tmp2[i+1][j]=tmp2[i+1][j]+error*7/48;}
            if (i+2<size){tmp2[i+2][j]=tmp2[i+2][j]+error*5/48;}
            if (j-1>=0&&i+1<size){tmp2[i+1][j-1]=tmp2[i+1][j-1]+error*5/48;}
            if (j-1>=0&&i+2<size){tmp2[i+2][j-1]=tmp2[i+2][j-1]+error*3/48;}
            if (j-2>=0&&i+1<size){tmp2[i+1][j-2]=tmp2[i+1][j-2]+error*3/48;}
            if (j-2>=0&&i+2<size){tmp2[i+2][j-2]=tmp2[i+2][j-2]+error*1/48;}
        }
    }
    // Stucki's matrix
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++){
            // compare the threshold and get the error
            if (tmp3[i][j]>=0.5) {
                error = tmp3[i][j]-1;
                imagedata_out3[i][j] = (unsigned char) 0;
            }
            else {
                error = tmp3[i][j]-0;
                imagedata_out3[i][j] = (unsigned char) 255;
            }
            // distribute the error
            if (j+1<size){tmp3[i][j+1]=tmp3[i][j+1]+error*8/42;}
            if (j+1<size&&i+1<size){tmp3[i+1][j+1]=tmp3[i+1][j+1]+error*4/42;}
            if (j+1<size&&i+2<size){tmp3[i+2][j+1]=tmp3[i+2][j+1]+error*2/42;}
            if (j+2<size){tmp3[i][j+2]=tmp3[i][j+2]+error*4/42;}
            if (j+2<size&&i+1<size){tmp3[i+1][j+2]=tmp3[i+1][j+2]+error*2/42;}
            if (j+2<size&&i+2<size){tmp3[i+2][j+2]=tmp3[i+2][j+2]+error*1/42;}
            if (i+1<size){tmp3[i+1][j]=tmp3[i+1][j]+error*8/42;}
            if (i+2<size){tmp3[i+2][j]=tmp3[i+2][j]+error*4/42;}
            if (j-1>=0&&i+1<size){tmp3[i+1][j-1]=tmp3[i+1][j-1]+error*4/42;}
            if (j-1>=0&&i+2<size){tmp3[i+2][j-1]=tmp3[i+2][j-1]+error*2/42;}
            if (j-2>=0&&i+1<size){tmp3[i+1][j-2]=tmp3[i+1][j-2]+error*2/42;}
            if (j-2>=0&&i+2<size){tmp3[i+2][j-2]=tmp3[i+2][j-2]+error*1/42;}
        }
    }
    // output the three images at the same time
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_out, sizeof(unsigned char), size*size, file);
    fclose(file);

    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Cannot open file: " << argv[3] << endl;
        exit(1);
    }
    fwrite(imagedata_out2, sizeof(unsigned char), size*size, file);
    fclose(file);

    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Cannot open file: " << argv[4] << endl;
        exit(1);
    }
    fwrite(imagedata_out3, sizeof(unsigned char), size*size, file);
    fclose(file);

    return 0;
}