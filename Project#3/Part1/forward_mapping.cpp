//
//  main.cpp
//  3D camera forward mapping with preprocessing
//
//  created by Yi Zheng on 10/20/15.
//  version 4.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
using namespace std;
// define a structure array as 3D coordinates to store rgb value
struct
{   int r;
    int g;
    int b;
}pixel[200][200][200];

int main(int argc, char *argv[])

{
    FILE *file;
    // Define file pointer and variables
    ifstream ifile;

    // ofstream ofile;
    int size = 200; //set the image size
    int i,j;
    unsigned char dataraw[size][size][3];
    unsigned char imagedata_new[size][size][3];

    // preprocessing
    // read baby image data
    file=fopen(argv[1],"rb");
    fread(dataraw, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    // store baby image
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            pixel[i][j][199].r= (int) dataraw[i][j][0];
            pixel[i][j][199].g= (int) dataraw[i][j][1];
            pixel[i][j][199].b= (int) dataraw[i][j][2];
        }
    }
    // read baby_cat image data
    file=fopen(argv[2],"rb");
    fread(dataraw, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    // store baby_cat image
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            pixel[199][j][199-i].r= (int) dataraw[i][j][0];
            pixel[199][j][199-i].g= (int) dataraw[i][j][1];
            pixel[199][j][199-i].b= (int) dataraw[i][j][2];
        }
    }
    // read baby_dog image data
    file=fopen(argv[3],"rb");
    fread(dataraw, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    // store baby_dog image
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            pixel[199-j][199][199-i].r= (int) dataraw[i][j][0];
            pixel[199-j][199][199-i].g= (int) dataraw[i][j][1];
            pixel[199-j][199][199-i].b= (int) dataraw[i][j][2];
        }
    }
    // read baby_panda image data
    file=fopen(argv[4],"rb");
    fread(dataraw, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    // store baby_panda image
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            pixel[0][199-j][199-i].r= (int) dataraw[i][j][0];
            pixel[0][199-j][199-i].g= (int) dataraw[i][j][1];
            pixel[0][199-j][199-i].b= (int) dataraw[i][j][2];
        }
    }

    // read baby_bear image data
    file=fopen(argv[5],"rb");
    fread(dataraw, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    // store baby_bear image
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            pixel[j][0][199-i].r= (int) dataraw[i][j][0];
            pixel[j][0][199-i].g= (int) dataraw[i][j][1];
            pixel[j][0][199-i].b= (int) dataraw[i][j][2];
        }
    }

    int sx,sy,sz;
    int density = 500; // pixel density
    double inv = 1.0/density;
    int sxi,syi,x,y;
    double dx,dy,dz;
    double cx=0.5*size*inv;  // translation length for 2D image
    double cy=0.5*size*inv;
    double dxc,dyc,dzc;
    double dxi,dyi;
    double f=sqrt(3); // focal length

    // forward mapping for face #1
    sz=199;
    for(sx=0;sx<200;sx++) {
        for (sy=0;sy<200;sy++) {
            // transform from the image coordinates to Cartesian coordinate
            dz=0.01*(sz+1)-1;
            dx=0.01*(sx+1)-1;
            dy=0.01*(sy+1)-1;
            // extrinsic camera matirx
            dxc=(-1/sqrt(2))*dx+(1/sqrt(2))*dy;
            dyc=(1/sqrt(6))*dx+(1/sqrt(6))*dy+(-2/sqrt(6))*dz;
            dzc=(-1/sqrt(3))*dx+(-1/sqrt(3))*dy+(-1/sqrt(3))*dz+(15/sqrt(3));
            // intrinsic camera matrix
            dxi=f*dxc/dzc+cx;
            dyi=f*dyc/dzc+cy;
            // truncating the fractional address
            sxi=int(dxi*density);
            syi=int(dyi*density);
            // transform from Cartesian coordinate to the image coordinates
            x=199-syi;
            y=sxi;
            if(x>=0&&x<size&&y>=0&&y<size)
            {
                imagedata_new[x][y][0] = (unsigned char)pixel[sx][sy][sz].r;
                imagedata_new[x][y][1] = (unsigned char)pixel[sx][sy][sz].g;
                imagedata_new[x][y][2] = (unsigned char)pixel[sx][sy][sz].b;
            }
        }
    }
    // forward mapping for face #2
    sx=199;
    for(sz=0;sz<200;sz++) {
        for (sy=0;sy<200;sy++) {
            // transform from the image coordinates to Cartesian coordinate
            dz=0.01*(sz+1)-1;
            dx=0.01*(sx+1)-1;
            dy=0.01*(sy+1)-1;
            // extrinsic camera matirx
            dxc=(-1/sqrt(2))*dx+(1/sqrt(2))*dy;
            dyc=(1/sqrt(6))*dx+(1/sqrt(6))*dy+(-2/sqrt(6))*dz;
            dzc=(-1/sqrt(3))*dx+(-1/sqrt(3))*dy+(-1/sqrt(3))*dz+(15/sqrt(3));
            // intrinsic camera matrix
            dxi=f*dxc/dzc+cx;
            dyi=f*dyc/dzc+cy;
            // truncating the fractional address
            sxi=int(dxi*density);
            syi=int(dyi*density);
            // transform from Cartesian coordinate to the image coordinates
            x=199-syi;
            y=sxi;
            if(x>=0&&x<size&&y>=0&&y<size)
            {
                imagedata_new[x][y][0] = (unsigned char)pixel[sx][sy][sz].r;
                imagedata_new[x][y][1] = (unsigned char)pixel[sx][sy][sz].g;
                imagedata_new[x][y][2] = (unsigned char)pixel[sx][sy][sz].b;
            }
        }
    }
    // forward mapping for face #3
    sy=199;
    for(sz=0;sz<200;sz++) {
        for (sx=0;sx<200;sx++) {
            // transform from the image coordinates to Cartesian coordinate
            dz=0.01*(sz+1)-1;
            dx=0.01*(sx+1)-1;
            dy=0.01*(sy+1)-1;
            // extrinsic camera matirx
            dxc=(-1/sqrt(2))*dx+(1/sqrt(2))*dy;
            dyc=(1/sqrt(6))*dx+(1/sqrt(6))*dy+(-2/sqrt(6))*dz;
            dzc=(-1/sqrt(3))*dx+(-1/sqrt(3))*dy+(-1/sqrt(3))*dz+(15/sqrt(3));
            // intrinsic camera matrix
            dxi=f*dxc/dzc+cx;
            dyi=f*dyc/dzc+cy;
            // truncating the fractional address
            sxi=int(dxi*density);
            syi=int(dyi*density);
            // transform from Cartesian coordinate to the image coordinates
            x=199-syi;
            y=sxi;
           if(x>=0&&x<size&&y>=0&&y<size)
            {
                imagedata_new[x][y][0] = (unsigned char)pixel[sx][sy][sz].r;
                imagedata_new[x][y][1] = (unsigned char)pixel[sx][sy][sz].g;
                imagedata_new[x][y][2] = (unsigned char)pixel[sx][sy][sz].b;
            }
        }
    }

    // output image
    if (!(file=fopen(argv[6],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(imagedata_new, sizeof(unsigned char), size*size*3, file);
    fclose(file);
    return 0;
}
