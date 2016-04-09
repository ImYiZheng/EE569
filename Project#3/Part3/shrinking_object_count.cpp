//
//  main.cpp
//  counting the number of objests after shrinking
//
//  created by Yi Zheng on 10/27/15.
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
    //int bytesPerPixel = 1;
    int size_h = 108;
    int size_w = 91;
    // Allocate image data array
    unsigned char imagedata[size_h][size_w];
    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(imagedata, sizeof(unsigned char), size_h*size_w, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    unsigned char imagedata_out[size_h][size_w];
    int tmp_w[size_h][size_w];
    int input[size_h][size_w];
    int w_m[size_h][size_w];
    // shrink conditional mark patterns
    int con_mask[58] = {1000000,10000,100,1,
            10000000,100000,1000,10,
            11000000,1100000,110000,11000,1100,110,11,10000001,
            11000001,1110000,11100,111,
            10110000,10100001,1101000,11000010,
            11100000,111000,1110,10000011,
            10110001,1101100,
            11110000,11100001,1111000,111100,11110,1111,10000111,11000011,
            11110001,1111100,11111,11000111,
            11100011,11111000,111110,10001111,
            11110011,11100111,11111100,11111001,111110,11111,10011111,11001111,
            11110111,11111101,111111,11011111};
    // shrink and thin unconditional mark patterns
    int uncon_mask[114] = {1000000,10000,
            10,10000000,
            11000000,1100000,110000,11000,1100,110,11,10000001,
            1101000,10110000,10100001,11000010,
            1100100,11000100,11100100,11001,110001,111001,1001100,1000110,1001110,10011,10010001,10010011,
            111000,11111111,
            10101000,10111100,11101001,10001010,11001011,10011110,101010,1111010,101111,10100010,10100111,11110010,
            10100100,10110101,101001,1101101,1001010,1011011,10010010,11010110,
            1010001,1010010,1010011,1010100,1010101,1010110,1010111,11111001,11111010,11111011,11111100,11111101,11111110,11111111,
            1010100,10010100,11010100,10101,1010101,10010101,11010101,1111110,10111110,11111110,111111,1111111,10111111,11111111,
            10101,100101,110101,1000101,1010101,1100101,1110101,10011111,10101111,10111111,11001111,11011111,11101111,11111111,
            1000101,1001001,1001101,1010001,1010101,1011001,1011101,11100111,11101011,11101111,11110011,11110111,11111011,11111111};
    int i,j,k,l;
    int count=0;
    int label[size_h][size_w];
    for (i=0;i<size_h;i++) {
        for (j=0;j<size_w;j++) {
            input[i][j] = (int)imagedata[i][j]/255;       // white circles
            //tmp_w[i][j] = 1-(int)imagedata[i][j]/255;     // black dots
            label[i][j]=0; // set the label matrix as 0's
        }
    }
   // hole-filling filter
    for (i=1;i<size_h-1;i++) {
        for (j=1;j<size_w-1;j++) {
            if (input[i][j]==0){
                if (input[i][j+1]==1&&input[i-1][j]==1&&input[i][j-1]==1&&input[i+1][j]==1) {
                    tmp_w[i][j] = 1;
                }
                else{
                    tmp_w[i][j] = input[i][j];
                }
            }
            else{
                tmp_w[i][j] = input[i][j];
            }
        }
    }

   for(l=0;l<7;l++)    // iterations
    {
        // initial the M matrix as 0's
        for (i=0;i<size_h;i++) {
            for (j=0;j<size_w;j++) {
                w_m[i][j] = 0;
            }
        }
        // get M (pixel to erase)- conditional matrix
        for (i=1;i<size_h-1;i++) {
            for (j=1;j<size_w-1;j++) {
                if (tmp_w[i][j] == 1) {
                    for (k=0;k<58;k++) {
                        if (10000000*tmp_w[i][j+1]+1000000*tmp_w[i-1][j+1]+100000*tmp_w[i-1][j]+10000*tmp_w[i-1][j-1]+1000*tmp_w[i][j-1]+100 * tmp_w[i+1][j-1]+10* tmp_w[i+1][j]+tmp_w[i+1][j+1]==con_mask[k])
                        {
                            w_m[i][j] = 1; // hit the conditional filter, set M as 1
                        }
                    }
                }
            }
        }
        // decide whether to erase- unconditional matrix
        for (i=1;i<size_h-1;i++) {
            for (j=1;j<size_w-1;j++) {
                if (w_m[i][j] == 1)
                {
                    for (k=0;k<114;k++) {
                        if (10000000*w_m[i][j+1]+1000000*w_m[i-1][j+1]+100000*w_m[i-1][j]+10000*w_m[i-1][j-1]+1000*w_m[i][j-1]+100*w_m[i+1][j-1]+10*w_m[i+1][j]+w_m[i+1][j+1]==uncon_mask[k])
                        {
                            tmp_w[i][j] = 1; // hit the unconditioanal filter, remain the pixel
                            break;
                        }
                        else
                        {
                            tmp_w[i][j] = 0; // miss the unconditional filter, erase the pixel
                        }
                    }
                }
            }
        }
    }
    // output image
    for (i=0;i<size_h;i++) {
        for (j = 0; j < size_w; j++) {
            if (tmp_w[i][j] == 1) {
                imagedata_out[i][j] = (unsigned char) 0;
            }
            else {
                imagedata_out[i][j] = (unsigned char) 255;
            }
        }
    }
    // counting the number of objects
    int id = 1; // initial the label number as 1
    for (i=1;i<size_h-1;i++) {
        for (j=1;j<size_w-1;j++) {
            if(tmp_w[i][j]==1)
            {
                if(label[i][j+1]!=0){label[i][j]=label[i][j+1];continue;}
                if(label[i-1][j+1]!=0){label[i][j]=label[i-1][j+1];continue;}
                if(label[i-1][j]!=0){label[i][j]=label[i-1][j+1];continue;}
                if(label[i-1][j-1]!=0){label[i][j]=label[i-1][j-1];continue;}
                if(label[i][j-1]!=0){label[i][j]=label[i][j-1];continue;}
                if(label[i+1][j-1]!=0){label[i][j]=label[i+1][j-1];continue;}
                if(label[i+1][j]!=0){label[i][j]=label[i+1][j];continue;}
                if(label[i+1][j+1]!=0){label[i][j]=label[i+1][j+1];continue;}
                label[i][j] = id;
                id++;
            }
        }
    }
/   // output the image
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_out, sizeof(unsigned char), size_h*size_w, file);
    fclose(file);
    return 0;
}