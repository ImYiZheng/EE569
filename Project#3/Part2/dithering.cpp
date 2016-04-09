//
//  main.cpp
//  half-tone using dithering
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

    unsigned char imagedata_out[size][size];
    unsigned char imagedata_out2[size][size];
    unsigned char imagedata_out3[size][size];
    int ind1[size][size];
    int ind2[size][size];
    double tmp[size][size];

    // Bayer index matrices
    int i_4[4][4] = {{5,9,6,10},{13,1,14,2},{7,11,4,8},{15,3,12,0}};
    int i_8[8][8] = {{21,37,25,41,22,38,26,42},{53,5,57,9,54,6,58,10},{29,45,17,33,30,46,18,34},{61,13,49,1,62,14,50,2},
                     {23,39,27,43,20,36,24,40},{55,7,59,11,52,4,56,8},{31,47,19,35,28,44,16,32},{63,15,51,3,60,12,48,0}};
    int N = 4; // the size of Bayer matrix
    int N2 = 8; // the size of Bayer matrix 2
    int i,j,m,n;

    // normalize and transform the image from display-based to pringting based
    for (i=0;i<size;i++) {
        for (j=0;j<size;j++){
            tmp[i][j] = 1-(double)imagedata[i][j]/255;
        }
    }
    // dithering using I4
    for (i=0;i<size;i=i+N){
        for (j=0;j<size;j=j+N){
            for(m=0;m<N;m++){
                for(n=0;n<N;n++){
                    if(tmp[i+m][j+n]>(i_4[m%N][n%N]+0.5)/(N*N)) // threshold
                    {
                        imagedata_out[i+m][j+n] = (unsigned char)0; // over threshold, set black
                        ind1[i+m][j+n] = 1; // indicator for (0,85)
                    }
                    else
                    {
                        imagedata_out[i+m][j+n] = (unsigned char)255;// below threshold, set white
                        ind1[i+m][j+n] = 0; // indicator for (170,255)
                    }
                }
            }
        }
    }
    // dithering using I8
    for (i=0;i<size;i=i+N2){
        for (j=0;j<size;j=j+N2){
            for(m=0;m<N2;m++){
                for(n=0;n<N2;n++){
                    if(tmp[i+m][j+n]>(i_8[m%N2][n%N2]+0.5)/(N2*N2)) // threshold
                    {
                        imagedata_out2[i+m][j+n] = (unsigned char)0; // over threshold, set black
                        ind2[i+m][j+n] = 1; // indicator for the upper level in each region
                    }
                    else
                    {
                        imagedata_out2[i+m][j+n] = (unsigned char)255; // below threshold, set white
                        ind1[i+m][j+n] = 0; // indicator for the lowwer level in each region
                    }
                }
            }
        }
    }
    // dithering using four gray-levels
    for (i=0;i<size;i++) {
        for (j = 0; j < size; j++){
            if (ind1[i][j]==1) // (0.85) region
            {
                if (ind2[i][j]==1)
                {
                    imagedata_out3[i][j] = (unsigned char) 0;
                }
                else
                {
                    imagedata_out3[i][j] = (unsigned char) 85;
                }
            }
            else // (170,255) region
            {
                if (ind2[i][j]==1)
                {
                    imagedata_out3[i][j] = (unsigned char) 170;
                }
                else
                {
                    imagedata_out3[i][j] = (unsigned char) 255;
                }
            }
        }
    }

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