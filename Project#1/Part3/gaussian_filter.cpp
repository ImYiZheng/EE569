//
//  main.cpp
//  using medium filter first and then Gaussian filter to eliminate mixed noise
//
//  created by Yi Zheng on 9/13/15.
//  version 2.0

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
    int size1 = 512;
    int size2 = 512;
    int size;

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
            size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[size2][size1][bytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), size1*size2*bytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int N=9;	//N*N neighborhood
    int i,j,k,l,h;
    int m,n,x;
    int t;
    double sigma=3,sum_val,sum_wgt;
    //output three channels image
/*  unsigned char Imagedata_R[size2][size1][1];
    unsigned char Imagedata_G[size2][size1][1];
    unsigned char Imagedata_B[size2][size1][1];
    for(i=0;i<size2;i++)		//Red channel
    {
        for(j=0;j<size1;j++)
        {
            Imagedata_R[i][j][0]=Imagedata[i][j][0];
        }
    }
    for(i=0;i<size2;i++)		//Green channel
    {
        for(j=0;j<size1;j++)
        {
            Imagedata_G[i][j][0]=Imagedata[i][j][1];
        }
    }
    for(i=0;i<size2;i++)		//Blue channel
    {
        for(j=0;j<size1;j++)
        {
            Imagedata_B[i][j][0]=Imagedata[i][j][2];
        }
    }
*/
    //matrix extension
    unsigned char tmp1[size2][size1+N-1][bytesPerPixel];
    unsigned char tmp2[size2+N-1][size1+N-1][bytesPerPixel];
    for (l=0;l<3;l++)
    {
        for(i=0;i<=size2-1;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata[i][j-(N-1)/2][l];
            }
        }
        for(i=0;i<=size2-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata[i][(N-1)/2-j][l];
                tmp1[i][size1+(N-1)/2+j][l] = Imagedata[i][size1-((N-1)/2+j)][l];
            }
        }
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=0;j<=size1+(N-1)-1;j++)
            {
                tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
            }

        }
        for(j=0;j<=size1+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
                tmp2[size2+(N-1)/2+i][j][l] = tmp1[size2-((N-1)/2+i)][j][l];
            }
        }

    }

    //impulse noise filter using cross shape
    int a[2*N-1];
    int tmp_val;
    for(l=0;l<3;l++)
    {
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                x=0;
                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {
                            a[x]=(int)tmp2[i][j][l];
                            x++;
                        }
                        if(m!=0&&n==0)
                        {
                            a[x]=(int)tmp2[i+m][j][l];
                            x++;
                            a[x]=(int)tmp2[i-m][j][l];
                            x++;
                        }
                        if(m==0&&n!=0)
                        {
                            a[x]=(int)tmp2[i][j+n][l];
                            x++;
                            a[x]=(int)tmp2[i][j-n][l];
                            x++;
                        }
                    }
                }
                t=2*N-1;
                for(k=0;k<2*N-1;k++)
                {
                    for(h=0;h<=t-2;h++)
                    {
                        if(a[h]>a[h+1])
                        {
                            tmp_val=a[h];
                            a[h]=a[h+1];
                            a[h+1]=tmp_val;
                        }
                    }
                    t--;
                }
                Imagedata[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)a[N];
            }
        }
    }

    //matrix extension after eliminating impulse noise
    for (l=0;l<3;l++)
    {
        for(i=0;i<=size2-1;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata[i][j-(N-1)/2][l];
            }
        }
        for(i=0;i<=size2-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp1[i][j][l] = Imagedata[i][(N-1)/2-j][l];
                tmp1[i][size1+(N-1)/2+j][l] = Imagedata[i][size1-((N-1)/2+j)][l];
            }
        }
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=0;j<=size1+(N-1)-1;j++)
            {
                tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
            }

        }
        for(j=0;j<=size1+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
                tmp2[size2+(N-1)/2+i][j][l] = tmp1[size2-((N-1)/2+i)][j][l];
            }
        }

    }

    // Gaussian filter
    for(l=0;l<3;l++)
    {
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                sum_val=0;
                sum_wgt=0;
                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {
                            sum_val=sum_val+exp(-1*0*0/(2*sigma*sigma))*(int)tmp2[i][j][l];
                            sum_wgt=sum_wgt+exp(-1*0*0/(2*sigma*sigma));;
                        }
                        if(m!=0&&n==0)
                        {
                            sum_val=sum_val+exp(-1*m*m/(2*sigma*sigma))*(int)tmp2[i+m][j][l];
                            sum_wgt=sum_wgt+exp(-1*m*m/(2*sigma*sigma));
                            sum_val=sum_val+exp(-1*m*m/(2*sigma*sigma))*(int)tmp2[i-m][j][l];
                            sum_wgt=sum_wgt+exp(-1*m*m/(2*sigma*sigma));
                        }
                        if(m==0&&n!=0)
                        {
                            sum_val=sum_val+exp(-1*n*n/(2*sigma*sigma))*(int)tmp2[i][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*n*n/(2*sigma*sigma));
                            sum_val=sum_val+exp(-1*n*n/(2*sigma*sigma))*(int)tmp2[i][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*n*n/(2*sigma*sigma));
                        }
                        if(m!=0&&n!=0)
                        {
                            sum_val=sum_val+exp(-1*(m*m+n*n)/(2*sigma*sigma))*(int)tmp2[i+m][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*(m*m+n*n)/(2*sigma*sigma));
                            sum_val=sum_val+exp(-1*(m*m+n*n)/(2*sigma*sigma))*(int)tmp2[i+m][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*(m*m+n*n)/(2*sigma*sigma));
                            sum_val=sum_val+exp(-1*(m*m+n*n)/(2*sigma*sigma))*(int)tmp2[i-m][j+n][l];
                            sum_wgt=sum_wgt+exp(-1*(m*m+n*n)/(2*sigma*sigma));
                            sum_val=sum_val+exp(-1*(m*m+n*n)/(2*sigma*sigma))*(int)tmp2[i-m][j-n][l];
                            sum_wgt=sum_wgt+exp(-1*(m*m+n*n)/(2*sigma*sigma));
                        }
                    }
                }
                Imagedata[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)(sum_val/sum_wgt);
            }
        }
    }

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), size1*size2*3, file);
    fclose(file);
}
