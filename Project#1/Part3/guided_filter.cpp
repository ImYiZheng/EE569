//
//  main.cpp
//  using guided filter for denoising
//
//  created by Yi Zheng on 9/14/15.
//  version 1.0
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
    float a[size2][size1];
    float b[size2][size1];
    float sum_mean,sum_cor;
    float sum_a,sum_b,mean_a,mean_b;
    float mean,cor,var;
    int N=9;	//N*N neighborhood
    int i,j,k,l,h;
    int m,n,x;
    int t;
    float epsilon=850;
    //matrix extension by reflection
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
    int c[2*N-1];
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
                            c[x]=(int)tmp2[i][j][l];
                            x++;
                        }
                        if(m!=0&&n==0)
                        {
                            c[x]=(int)tmp2[i+m][j][l];
                            x++;
                            c[x]=(int)tmp2[i-m][j][l];
                            x++;
                        }
                        if(m==0&&n!=0)
                        {
                            c[x]=(int)tmp2[i][j+n][l];
                            x++;
                            c[x]=(int)tmp2[i][j-n][l];
                            x++;
                        }
                    }
                }
                t=2*N-1;
                for(k=0;k<2*N-1;k++)
                {
                    for(h=0;h<=t-2;h++)
                    {
                        if(c[h]>c[h+1])
                        {
                            tmp_val=c[h];
                            c[h]=c[h+1];
                            c[h+1]=tmp_val;
                        }
                    }
                    t--;
                }
                Imagedata[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)c[N];
            }
        }
    }
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
    //guided filter for denoising
    float tmp3[size2][size1+N-1];
    float tmp4[size2+N-1][size1+N-1];
    float tmp5[size2+N-1][size1+N-1];
    k=0;
    for(l=0;l<3;l++)
    {
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                sum_mean=0;
                sum_cor=0;
                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {
                            sum_mean=sum_mean+(int)tmp2[i][j][l];
                            sum_cor=sum_cor+(int)tmp2[i][j][l]*(int)tmp2[i][j][l];
                        }
                        if(m!=0&&n==0)
                        {
                            sum_mean=sum_mean+(int)tmp2[i+m][j][l];
                            sum_cor=sum_cor+(int)tmp2[i+m][j][l]*(int)tmp2[i+m][j][l];
                            sum_mean=sum_mean+(int)tmp2[i-m][j][l];
                            sum_cor=sum_cor+(int)tmp2[i-m][j][l]*(int)tmp2[i-m][j][l];
                        }
                        if(m==0&&n!=0)
                        {
                            sum_mean=sum_mean+(int)tmp2[i][j+n][l];
                            sum_cor=sum_cor+(int)tmp2[i][j+n][l]*(int)tmp2[i][j+n][l];
                            sum_mean=sum_mean+(int)tmp2[i][j-n][l];
                            sum_cor=sum_cor+(int)tmp2[i][j-n][l]*(int)tmp2[i][j-n][l];
                        }
                        if(m!=0&&n!=0)
                        {
                            sum_mean=sum_mean+(int)tmp2[i+m][j+n][l];
                            sum_cor=sum_cor+(int)tmp2[i+m][j+n][l]*(int)tmp2[i+m][j+n][l];
                            sum_mean=sum_mean+(int)tmp2[i-m][j+n][l];
                            sum_cor=sum_cor+(int)tmp2[i-m][j+n][l]*(int)tmp2[i-m][j+n][l];
                            sum_mean=sum_mean+(int)tmp2[i+m][j-n][l];
                            sum_cor=sum_cor+(int)tmp2[i+m][j-n][l]*(int)tmp2[i+m][j-n][l];
                            sum_mean=sum_mean+(int)tmp2[i-m][j-n][l];
                            sum_cor=sum_cor+(int)tmp2[i-m][j-n][l]*(int)tmp2[i-m][j-n][l];
                        }
                    }
                }
                mean=sum_mean/(N*N);
                cor=sum_cor/(N*N);
                var=cor-mean*mean;
                a[i-(N-1)/2][j-(N-1)/2]=var/(var+epsilon);
                b[i-(N-1)/2][j-(N-1)/2]=mean-mean*var/(var+epsilon);
            }
        }

        //store coefficient a
        for(i=0;i<=size2-1;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                tmp3[i][j] = a[i][j-(N-1)/2];
            }
        }
        for(i=0;i<=size2-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp3[i][j] = a[i][(N-1)/2-j];
                tmp3[i][size1+(N-1)/2+j] = a[i][size1-((N-1)/2+j)];
            }
        }
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=0;j<=size1+(N-1)-1;j++)
            {
                tmp4[i][j] = tmp3[i-(N-1)/2][j];
            }

        }
        for(j=0;j<=size1+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp4[i][j] = tmp3[(N-1)/2-i][j];
                tmp4[size2+(N-1)/2+i][j] = tmp3[size2-((N-1)/2+i)][j];
            }
        }
        //store coefficient b
        for(i=0;i<=size2-1;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                tmp3[i][j] = b[i][j-(N-1)/2];
            }
        }
        for(i=0;i<=size2-1;i++)
        {
            for(j=0;j<(N-1)/2;j++)
            {
                tmp3[i][j] = b[i][(N-1)/2-j];
                tmp3[i][size1+(N-1)/2+j] = b[i][size1-((N-1)/2+j)];
            }
        }
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=0;j<=size1+(N-1)-1;j++)
            {
                tmp5[i][j] = tmp3[i-(N-1)/2][j];
            }

        }
        for(j=0;j<=size1+(N-1)-1;j++)
        {
            for(i=0;i<(N-1)/2;i++)
            {
                tmp5[i][j] = tmp3[(N-1)/2-i][j];
                tmp5[size2+(N-1)/2+i][j] = tmp3[size2-((N-1)/2+i)][j];
            }
        }
        //calculate a mean and b mean
        for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
        {
            for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
            {
                sum_a=0;
                sum_b=0;
                for(m=0;m<=(N-1)/2;	m++)
                {
                    for(n=0;n<=(N-1)/2;n++)
                    {
                        if(m==0&&n==0)
                        {
                            sum_a=sum_a+tmp4[i][j];
                            sum_b=sum_b+tmp5[i][j];
                        }
                        if(m!=0&&n==0)
                        {
                            sum_a=sum_a+tmp4[i+m][j];
                            sum_b=sum_b+tmp5[i+m][j];
                            sum_a=sum_a+tmp4[i-m][j];
                            sum_b=sum_b+tmp5[i-m][j];
                        }
                        if(m==0&&n!=0)
                        {
                            sum_a=sum_a+tmp4[i][j+n];
                            sum_b=sum_b+tmp5[i][j+n];
                            sum_a=sum_a+tmp4[i][j-n];
                            sum_b=sum_b+tmp5[i][j-n];
                        }
                        if(m!=0&&n!=0)
                        {
                            sum_a=sum_a+tmp4[i+m][j+n];
                            sum_a=sum_a+tmp4[i-m][j+n];
                            sum_a=sum_a+tmp4[i+m][j-n];
                            sum_a=sum_a+tmp4[i-m][j-n];
                            sum_b=sum_b+tmp5[i+m][j+n];
                            sum_b=sum_b+tmp5[i-m][j+n];
                            sum_b=sum_b+tmp5[i+m][j-n];
                            sum_b=sum_b+tmp5[i-m][j-n];
                        }
                    }
                }
                mean_a=sum_a/(N*N);
                mean_b=sum_b/(N*N);
                Imagedata[i-(N-1)/2][j-(N-1)/2][l]=(unsigned char)((int)Imagedata[i-(N-1)/2][j-(N-1)/2][l]*mean_a+mean_b);
            }
        }
    }
    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), (size1)*(size2)*3, file);
    fclose(file);
}
