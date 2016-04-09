//
//  main.cpp
//  thinning and skeletonizing after preprocessing
//
//  created by Yi Zheng on 10/28/15.
//	version 3.0
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
    int size_h = 275;
    int size_w = 360;
    // Allocate image data array
    unsigned char imagedata[size_h][size_w];
    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(imagedata, sizeof(unsigned char), size_h*size_w, file);
    fclose(file);

    unsigned char imagedata_out[size_h][size_w];
    int tmp_w[size_h][size_w];
    int input[size_h][size_w];
    int input2[size_h][size_w];
    int w_m[size_h][size_w];
    // shrink conditional mark patterns
    int con_mask_thin[46] = {10100000,101000,1010,10000010,
            11000001,1110000,11100,111,
            10110000,10100001,1101000,11000010,
            11100000,111000,1110,10000011,
            10110001,1101100,
            11110000,11100001,1111000,111100,11110,1111,10000111,11000011,
            11110001,1111100,11111,11000111,
            11100011,11111000,111110,10001111,
            11110011,11100111,11111100,11111001,111110,11111,10011111,11001111,
            11110111,11111101,111111,11011111};
    int con_mask_skeleton[40] = {10100000,101000,1010,10000010,
            11000001,1110000,11100,111,
            11110000,11100001,1111000,111100,11110,1111,10000111,11000011,
            11110001,1111100,11111,11000111,
            11100011,11111000,111110,10001111,
            11110011,11100111,11111100,11111001,111110,11111,10011111,11001111,
            11110111,11111101,111111,11011111,
            11111011,11111110,10111111,11101111};
    // shrink and thin unconditional mark patterns
    int uncon_mask_thin[114] = {1000000,10000,
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
    int uncon_mask_skeleton[88] = {1,100,1000000,10000,
            10,10000000,1000,100000,
            10100000,101000,10000010,1010,
            111000,11111111,11111111,10000011,
            10101000,11111111,101010,11111111,10001010,11111111,10100010,11111111,
            1010001,1010010,1010011,1010100,1010101,1010110,1010111,11111001,11111010,11111011,11111100,11111101,11111110,11111111,
            1010100,10010100,11010100,10101,1010101,10010101,11010101,1111110,10111110,11111110,111111,1111111,10111111,11111111,
            10101,100101,110101,1000101,1010101,1100101,1110101,10011111,10101111,10111111,11001111,11011111,11101111,11111111,
            1000101,1001001,1001101,1010001,1010101,1011001,1011101,11100111,11101011,11101111,11110011,11110111,11111011,11111111,
            10100100,10110101,101001,1101101,1001010,1011011,10010010,11010110};

    int i,j,k,l;
    for (i=0;i<size_h;i++) {
        for (j=0;j<size_w;j++) {
            //input[i][j] = (int)imagedata[i][j]/1;
            input[i][j] = 1-(int)imagedata[i][j]/255;
        }
    }
    // pre-processing
    // hole-filling
    for (i=1;i<size_h-1;i++) {
        for (j=1;j<size_w-1;j++) {
            if (input[i][j]==0){
                if (input[i][j+1]==1&&input[i-1][j]==1&&input[i][j-1]==1&&input[i+1][j]==1) {
                    input2[i][j] = 1;
                }
                else{
                    input2[i][j] = input[i][j];
                }
            }
            else{
                input2[i][j] = input[i][j];
            }
        }
    }
    // hole filling and contour smoothing
    int close[size_h+2][size_w+2];
    int close2[size_h+4][size_w+4];
    for (i=0;i<size_h+2;i++) {
        for (j=0;j<size_w+2;j++) {
            close[i][j] = 0;
        }
    }
    // dilation
    // 1 left-up
    for (i=0;i<size_h;i++) {
        for (j=0;j<size_w;j++) {
            close[i][j] = close[i][j]||input2[i][j];
        }
    }
    // 2 up
    for (i=0;i<size_h;i++) {
        for (j=1;j<size_w+1;j++) {
            close[i][j] = close[i][j]||input2[i][j-1];
        }
    }
    // 3 right-up
    for (i=0;i<size_h;i++) {
        for (j=2;j<size_w+2;j++) {
            close[i][j] = close[i][j]||input2[i][j-2];
        }
    }
    // 4 left
    for (i=1;i<size_h+1;i++) {
        for (j=0;j<size_w;j++) {
            close[i][j] = close[i][j]||input2[i-1][j];
        }
    }
    // 5 origin
    for (i=1;i<size_h+1;i++) {
        for (j=1;j<size_w+1;j++) {
            close[i][j] = close[i][j]||input2[i-1][j-1];
        }
    }
    // 6 right
    for (i=1;i<size_h+1;i++) {
        for (j=2;j<size_w+2;j++) {
            close[i][j] = close[i][j]||input2[i-1][j-2];
        }
    }
    // 7 left-down
    for (i=2;i<size_h+2;i++) {
        for (j=0;j<size_w;j++) {
            close[i][j] = close[i][j]||input2[i-2][j];
        }
    }
    // 8 down
    for (i=2;i<size_h+2;i++) {
        for (j=1;j<size_w+1;j++) {
            close[i][j] = close[i][j]||input2[i-2][j-1];
        }
    }
    // 9 right-down
    for (i=2;i<size_h+2;i++) {
        for (j=2;j<size_w+2;j++) {
            close[i][j] = close[i][j]||input2[i-2][j-2];
        }
    }

    // erosion
    for (i=0;i<size_h+2;i++) {
        for (j=0;j<size_w+2;j++) {
            close2[i][j] = close[i][j];
        }
    }
    // 1 left-up
    for (i=0;i<size_h+2;i++) {
        for (j=0;j<size_w+2;j++) {
            close2[i][j] = close2[i][j]&&close[i][j];
        }
    }
    // 2 up
    for (i=0;i<size_h+2;i++) {
        for (j=1;j<size_w+3;j++) {
            close2[i][j] = close2[i][j]&&close[i][j-1];
        }
    }
    // 3 right-up
    for (i=0;i<size_h+2;i++) {
        for (j=2;j<size_w+4;j++) {
            close2[i][j] = close2[i][j]&&close[i][j-2];
        }
    }
    // 4 left
    for (i=1;i<size_h+3;i++) {
        for (j=0;j<size_w+2;j++) {
            close2[i][j] = close2[i][j]&&close[i-1][j];
        }
    }
    // 5 origin
    for (i=1;i<size_h+3;i++) {
        for (j=1;j<size_w+3;j++) {
            close2[i][j] = close2[i][j]&&close[i-1][j-1];
        }
    }
    // 6 right
    for (i=1;i<size_h+3;i++) {
        for (j=2;j<size_w+4;j++) {
            close2[i][j] = close2[i][j]&&close[i-1][j-2];
        }
    }
    // 7 left-down
    for (i=2;i<size_h+4;i++) {
        for (j=0;j<size_w+2;j++) {
            close2[i][j] = close2[i][j]&&close[i-2][j];
        }
    }
    // 8 down
    for (i=2;i<size_h+4;i++) {
        for (j=1;j<size_w+3;j++) {
            close2[i][j] = close2[i][j]&&close[i-2][j-1];
        }
    }
    // 9 right-down
    for (i=2;i<size_h+4;i++) {
        for (j=2;j<size_w+4;j++) {
            close2[i][j] = close2[i][j]&&close[i-2][j-2];
        }
    }
    // after preprocessing
    for (i=2;i<size_h+2;i++) {
        for (j=2;j<size_w+2;j++) {
            tmp_w[i - 2][j - 2] = close2[i][j];
        }}
/*
    // without preprocessing
    for (i=0;i<size_h;i++) {
        for (j=0;j<size_w;j++) {
            tmp_w[i][j] = input2[i][j];
        }}
*/
    // thin
    for(l=0;l<45;l++)    // iterations
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
                    for (k=0;k<46;k++) {
                        if (10000000*tmp_w[i][j+1]+1000000*tmp_w[i-1][j+1]+100000*tmp_w[i-1][j]+10000*tmp_w[i-1][j-1]+1000*tmp_w[i][j-1]+100 * tmp_w[i+1][j-1]+10* tmp_w[i+1][j]+tmp_w[i+1][j+1]==con_mask_thin[k])
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
                        if (10000000*w_m[i][j+1]+1000000*w_m[i-1][j+1]+100000*w_m[i-1][j]+10000*w_m[i-1][j-1]+1000*w_m[i][j-1]+100*w_m[i+1][j-1]+10*w_m[i+1][j]+w_m[i+1][j+1]==uncon_mask_thin[k])
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
                imagedata_out[i][j] = (unsigned char) 255;
            }
            else {
                imagedata_out[i][j] = (unsigned char) 0;
            }
        }
    }
/*
   // skeleton
    for(l=0;l<45;l++)    // iterations
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
                    for (k=0;k<40;k++) {
                        if (10000000*tmp_w[i][j+1]+1000000*tmp_w[i-1][j+1]+100000*tmp_w[i-1][j]+10000*tmp_w[i-1][j-1]+1000*tmp_w[i][j-1]+100 * tmp_w[i+1][j-1]+10* tmp_w[i+1][j]+tmp_w[i+1][j+1]==con_mask_skeleton[k])
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
                    for (k=0;k<88;k++) {
                        if (10000000*w_m[i][j+1]+1000000*w_m[i-1][j+1]+100000*w_m[i-1][j]+10000*w_m[i-1][j-1]+1000*w_m[i][j-1]+100*w_m[i+1][j-1]+10*w_m[i+1][j]+w_m[i+1][j+1]==uncon_mask_skeleton[k])
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
    */
    for (i=0;i<size_h;i++) {
        for (j = 0; j < size_w; j++) {
            if (tmp_w[i][j] == 1) {
                imagedata_out[i][j] = (unsigned char) 255; // set binary 1 as black
            }
            else {
                imagedata_out[i][j] = (unsigned char) 0; // set binary 0 as white
            }
        }
    }
    // output image
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_out, sizeof(unsigned char), size_h*size_w, file);
    fclose(file);

    return 0;
}