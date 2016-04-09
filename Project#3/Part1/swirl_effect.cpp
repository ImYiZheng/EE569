//
//  main.cpp
//  image swirl effect via geometrical manipulation
//
//  created by Yi Zheng on 10/18/15.
//	version 1.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <math.h>


using namespace std;
using namespace cv;

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

    double dx,dy;
    int sx,sy,p,q;
    // translation length
    double x0 = 0.5*(size-1);
    double y0 = 0.5*(size-1);
    double r,angle,tx,ty,a,b;
    unsigned char imagedata_new[size][size][3];
    int k;
    for(sx=0;sx<size;sx++)
        for(sy=0;sy<size;sy++)
        {
            // translation
            dx = sx-x0;
            dy = sy-y0;
            // find the radial coordinate
            r = sqrt(dx*dx+dy*dy);
            // find the rotation angle
            angle = 3.1415*r/size;
            // rotation
            tx = dx*cos(angle)+dy*sin(angle) + x0;
            ty = -1*dx*sin(angle)+dy*cos(angle) + y0;
            if (tx >=0 && tx<=size && ty>=0 && ty<=size)  // the pixel inside the input image after reverse mapping
            {
                // bilinear interpolation to find the pixel value for each pixel in the out image
                a = tx-int(tx);
                b = ty-int(ty);
                p = int(tx);
                q = int(ty);
                for(k=0;k<3;k++)
                {
                    imagedata_new[sx][sy][k] =(unsigned char) ((1-a)*(1-b)*(int)imagedata[p][q][k]+b*(1-a)*(int)imagedata[p][q+1][k]+a*(1-b)*(int)imagedata[p+1][q][k]+a*b*(int)imagedata[p+1][q+1][k]);
                }
            }
            else{
                for(k=0;k<3;k++)
                {
                    // // the pixel outside the input image after reverse mapping,set RGB value 0
                    imagedata_new[sx][sy][k] =(unsigned char)0;
                }
            }

        }
/   // output the image wit swirl effect
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_new, sizeof(unsigned char), size*size*bytesPerPixel, file);
    fclose(file);
    return 0;
}