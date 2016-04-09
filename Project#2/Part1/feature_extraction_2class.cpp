//
//  main.cpp
//  texture_classification for two classes
//
//  created by Yi Zheng on 9/27/15.
//  version 1.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

int main(void)

{
    // Define file pointer and variables
    ifstream ifile;
    ofstream ofile;
    int size = 128; //set the image size
    int h,i,j,k,l,m,n;
    int x,y,z;
    int val;
    int N=5;	//N*N filter size
    // input the samples
    string raw[] = { "grass_01.raw","grass_02.raw","grass_03.raw","grass_04.raw","grass_05.raw","grass_06.raw","grass_07.raw","grass_08.raw","grass_09.raw","grass_10.raw","grass_11.raw","grass_12.raw",
                     "grass_13.raw","grass_14.raw","grass_15.raw","grass_16.raw","grass_17.raw","grass_18.raw","grass_19.raw","grass_20.raw","grass_21.raw","grass_22.raw","grass_23.raw","grass_24.raw",
                     "grass_25.raw","grass_26.raw","grass_27.raw","grass_28.raw","grass_29.raw","grass_30.raw","grass_31.raw","grass_32.raw","grass_33.raw","grass_34.raw","grass_35.raw","grass_36.raw",
                     "straw_01.raw","straw_02.raw","straw_03.raw","straw_04.raw","straw_05.raw","straw_06.raw","straw_07.raw","straw_08.raw","straw_09.raw","straw_10.raw","straw_11.raw","straw_12.raw",
                     "straw_13.raw","straw_14.raw","straw_15.raw","straw_16.raw","straw_17.raw","straw_18.raw","straw_19.raw","straw_20.raw","straw_21.raw","straw_22.raw","straw_23.raw","straw_24.raw",
                     "straw_25.raw","straw_26.raw","straw_27.raw","straw_28.raw","straw_29.raw","straw_30.raw","straw_31.raw","straw_32.raw","straw_33.raw","straw_34.raw","straw_35.raw","straw_36.raw",
                     "unknown_01.raw","unknown_02.raw","unknown_03.raw","unknown_04.raw","unknown_05.raw","unknown_06.raw","unknown_07.raw","unknown_08.raw","unknown_09.raw","unknown_10.raw","unknown_11.raw","unknown_12.raw",
                     "unknown_13.raw","unknown_14.raw","unknown_15.raw","unknown_16.raw","unknown_17.raw","unknown_18.raw","unknown_19.raw","unknown_20.raw","unknown_21.raw","unknown_22.raw","unknown_23.raw","unknown_24.raw"};
    char dataraw[size][size];
    int imagedata[size][size];
    int tmp1[size][size+N-1];
    int tmp2[size+N-1][size+N-1];
    double c[96][25]; // 96 output samples
    int a[5][5] = {{1,4,6,4,1},{-1,-2,0,2,1},{-1,0,2,0,-1},{-1,2,0,-2,1},{1,-4,6,-4,1}}; // 5 1D filters
    int b[5][5];
    double energy;
    // read each image, extract the features and then output the sample
    for(h=0;h<96;h++)
    {
        // read data
        ifile.open(raw[h], ios::in | ios::binary);
        ifile.read((char *) dataraw, sizeof(dataraw));
        ifile.close();
        // convert the value to int type
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if ((int) dataraw[i][j] > 0) {
                    imagedata[i][j] = (int) dataraw[i][j];
                }
                else {
                    imagedata[i][j] = 255 + (int) dataraw[i][j];
                }
            }
        }
       // matrix extension
        for(i=0;i<=size-1;i++) {
            for(j=(N-1)/2;j<=size-1+(N-1)/2;j++) {
                tmp1[i][j] = imagedata[i][j-(N-1)/2];
            }
        }
        for(i=0;i<=size-1;i++) {
            for(j=0;j<(N-1)/2;j++) {
                tmp1[i][j] = imagedata[i][(N-1)/2-j];
                tmp1[i][size+(N-1)/2+j] = imagedata[i][size-((N-1)/2+j)];
            }
        }
        for(i=(N-1)/2;i<=size-1+(N-1)/2;i++) {
            for(j=0;j<=size+(N-1)-1;j++) {
                tmp2[i][j] = tmp1[i-(N-1)/2][j];
            }
        }
        for(j=0;j<=size+(N-1)-1;j++) {
            for(i=0;i<(N-1)/2;i++) {
                tmp2[i][j] = tmp1[(N-1)/2-i][j];
                tmp2[size+(N-1)/2+i][j] = tmp1[size-((N-1)/2+i)][j];
            }
        }
        // calculate the mean and substract it
        for(i=(N-1)/2;i<=size-1+(N-1)/2;i++) {
            for(j=(N-1)/2;j<=size-1+(N-1)/2;j++) {
                val=0;
                for(m=0;m<=(N-1)/2;m++){
                    for(n=0;n<=(N-1)/2;n++) {
                        if (m==0&&n==0) {
                            val=val+tmp2[i][j];
                        }
                        if (m!=0&&n==0) {
                            val=val+tmp2[i+m][j];
                            val=val+tmp2[i-m][j];
                        }
                        if (m==0&&n!=0) {
                            val=val+tmp2[i][j+n];
                            val=val+tmp2[i][j-n];
                        }
                        if(m!=0&&n!=0) {
                            val=val+tmp2[i+m][j+n];
                            val=val+tmp2[i+m][j-n];
                            val=val+tmp2[i-m][j+n];
                            val=val+tmp2[i-m][j-n];
                        }
                    }
                }
                val=val/(N*N);
                imagedata[i-(N-1)/2][j-(N-1)/2]=imagedata[i-(N-1)/2][j-(N-1)/2]-val;
            }
        }
        // matrix extension for Laws' filter banks
       for(i=0;i<=size-1;i++) {
            for(j=(N-1)/2;j<=size-1+(N-1)/2;j++) {
                tmp1[i][j] = imagedata[i][j-(N-1)/2];
            }
        }
        for(i=0;i<=size-1;i++) {
            for(j=0;j<(N-1)/2;j++) {
                tmp1[i][j] = imagedata[i][(N-1)/2-j];
                tmp1[i][size+(N-1)/2+j] = imagedata[i][size-((N-1)/2+j)];
            }
        }
        for(i=(N-1)/2;i<=size-1+(N-1)/2;i++) {
            for(j=0;j<=size+(N-1)-1;j++) {
                tmp2[i][j] = tmp1[i-(N-1)/2][j];
            }
        }
        for(j=0;j<=size+(N-1)-1;j++) {
            for(i=0;i<(N-1)/2;i++) {
                tmp2[i][j] = tmp1[(N-1)/2-i][j];
                tmp2[size+(N-1)/2+i][j] = tmp1[size-((N-1)/2+i)][j];
            }
        }
        // filtering using Laws' filter banks
        z=0;
        for(i=0;i<5;i++)
        {
            for(j=0;j<5;j++)  // forming 25 Laws' filter banks
            {
                for(k=0;k<5;k++)
                {
                    for(l=0;l<5;l++)
                    {
                        b[k][l] = a[i][k]*a[j][l]; // forming the 5*5 filter
                    }
                }
                energy=0;
                for(x=(N-1)/2;x<=size-1+(N-1)/2;x++) {
                    for(y=(N-1)/2;y<=size-1+(N-1)/2;y++) {
                        val=0;
                        for(m=0;m<=(N-1)/2;m++){
                            for(n=0;n<=(N-1)/2;n++) {
                                if (m==0&&n==0) {
                                    val=val+b[2][2]*tmp2[x][y];
                                }
                                if (m!=0&&n==0) {
                                    val=val+b[2+m][2]*tmp2[x+m][y];
                                    val=val+b[2-m][2]*tmp2[x-m][y];
                                }
                                if (m==0&&n!=0) {
                                    val=val+b[2][2+n]*tmp2[x][y+n];
                                    val=val+b[2][2-n]*tmp2[x][y-n];
                                }
                                if(m!=0&&n!=0) {
                                    val=val+b[2+m][2+n]*tmp2[x+m][y+n];
                                    val=val+b[2+m][2-n]*tmp2[x+m][y-n];
                                    val=val+b[2-m][2+n]*tmp2[x-m][y+n];
                                    val=val+b[2-m][2-n]*tmp2[x-m][y-n];
                                }
                            }
                        }
                        energy=energy+val*val;
                    }
                }
                c[h][z] = energy/(size*size); // calculate the average energy for one feature
                z++;
            }
        }
    }
    //output the samples in csv format
    ofile.open("part_a.csv", ios::out | ios::trunc);
    for(i=0;i<96;i++) {
        for(j=0;j<25;j++) {
            ofile << c[i][j] << ","<<endl;
        }
    }
    ofile.close();
    return 0;
}
