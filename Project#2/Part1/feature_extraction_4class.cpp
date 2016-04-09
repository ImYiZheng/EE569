//
//  main.cpp
//  texture_classification for 4 classes
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
    int N=5; //N*N filter size
    // input the samples
    string raw[] = { "grass_01.raw","grass_02.raw","grass_03.raw","grass_04.raw","grass_05.raw","grass_06.raw","grass_07.raw","grass_08.raw","grass_09.raw","grass_10.raw","grass_11.raw","grass_12.raw",
            "grass_13.raw","grass_14.raw","grass_15.raw","grass_16.raw","grass_17.raw","grass_18.raw","grass_19.raw","grass_20.raw","grass_21.raw","grass_22.raw","grass_23.raw","grass_24.raw",
            "grass_25.raw","grass_26.raw","grass_27.raw","grass_28.raw","grass_29.raw","grass_30.raw","grass_31.raw","grass_32.raw","grass_33.raw","grass_34.raw","grass_35.raw","grass_36.raw",
            "grass_37.raw","grass_38.raw","grass_39.raw","grass_40.raw","grass_41.raw","grass_42.raw","grass_43.raw","grass_44.raw","grass_45.raw","grass_46.raw","grass_47.raw","grass_48.raw",
            "leather_01.raw","leather_02.raw","leather_03.raw","leather_04.raw","leather_05.raw","leather_06.raw","leather_07.raw","leather_08.raw","leather_09.raw","leather_10.raw","leather_11.raw","leather_12.raw",
            "leather_13.raw","leather_14.raw","leather_15.raw","leather_16.raw","leather_17.raw","leather_18.raw","leather_19.raw","leather_20.raw","leather_21.raw","leather_22.raw","leather_23.raw","leather_24.raw",
            "leather_25.raw","leather_26.raw","leather_27.raw","leather_28.raw","leather_29.raw","leather_30.raw","leather_31.raw","leather_32.raw","leather_33.raw","leather_34.raw","leather_35.raw","leather_36.raw",
            "leather_37.raw","leather_38.raw","leather_39.raw","leather_40.raw","leather_41.raw","leather_42.raw","leather_43.raw","leather_44.raw","leather_45.raw","leather_46.raw","leather_47.raw","leather_48.raw",
            "sand_01.raw","sand_02.raw","sand_03.raw","sand_04.raw","sand_05.raw","sand_06.raw","sand_07.raw","sand_08.raw","sand_09.raw","sand_10.raw","sand_11.raw","sand_12.raw",
            "sand_13.raw","sand_14.raw","sand_15.raw","sand_16.raw","sand_17.raw","sand_18.raw","sand_19.raw","sand_20.raw","sand_21.raw","sand_22.raw","sand_23.raw","sand_24.raw",
            "sand_25.raw","sand_26.raw","sand_27.raw","sand_28.raw","sand_29.raw","sand_30.raw","sand_31.raw","sand_32.raw","sand_33.raw","sand_34.raw","sand_35.raw","sand_36.raw",
            "sand_37.raw","sand_38.raw","sand_39.raw","sand_40.raw","sand_41.raw","sand_42.raw","sand_43.raw","sand_44.raw","sand_45.raw","sand_46.raw","sand_47.raw","sand_48.raw",
            "straw_01.raw","straw_02.raw","straw_03.raw","straw_04.raw","straw_05.raw","straw_06.raw","straw_07.raw","straw_08.raw","straw_09.raw","straw_10.raw","straw_11.raw","straw_12.raw",
            "straw_13.raw","straw_14.raw","straw_15.raw","straw_16.raw","straw_17.raw","straw_18.raw","straw_19.raw","straw_20.raw","straw_21.raw","straw_22.raw","straw_23.raw","straw_24.raw",
            "straw_25.raw","straw_26.raw","straw_27.raw","straw_28.raw","straw_29.raw","straw_30.raw","straw_31.raw","straw_32.raw","straw_33.raw","straw_34.raw","straw_35.raw","straw_36.raw",
            "straw_37.raw","straw_38.raw","straw_39.raw","straw_40.raw","straw_41.raw","straw_42.raw","straw_43.raw","straw_44.raw","straw_45.raw","straw_46.raw","straw_47.raw","straw_48.raw",
           };
    char dataraw[size][size];
    int imagedata[size][size];
    int tmp1[size][size+N-1];
    int tmp2[size+N-1][size+N-1];
    double c[192][25]; //192 output samples
    int a[5][5] = {{1,4,6,4,1},{-1,-2,0,2,1},{-1,0,2,0,-1},{-1,2,0,-2,1},{1,-4,6,-4,1}}; // 5 1D filters
    int b[5][5];
    double energy;
    // read each image, extract the features and then output the sample
    for(h=0;h<192;h++)
    {
        // read data
        ifile.open(raw[h],ios::in|ios::binary);
        ifile.read((char*)dataraw,sizeof(dataraw));
        ifile.close();
        // convert the value to int
        for(i=0;i<size;i++) {
            for(j=0;j<size;j++) {
                imagedata[i][j]=(int)dataraw[i][j];
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
            for(j=0;j<5;j++) // forming 25 Laws' filter banks
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
    // output samples in csv format
    ofile.open("part_b.csv", ios::out | ios::trunc);
    for(i=0;i<192;i++) {
        for(j=0;j<25;j++) {
            ofile << c[i][j]<< ","<<endl;
        }
    }
    ofile.close();
    return 0;
}
