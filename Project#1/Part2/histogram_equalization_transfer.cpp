//
//  main.cpp
//  histogram equalization based on transfer function method
//
//  created by Yi Zheng on 9/9/15.
//  version 1.0
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int size = 512;

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
    unsigned char Imagedata[size][size][bytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), size*size*bytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    int Imagedata_channel[size][size][3];
    int numofpixels[3][256]={0};
    int sumofpixels[3][256]={0};
    int i,j,k,chl;
    int sum;
    //convert the char value to int
    for(k=0;k<=2;k++)
    {
        for(i=0;i<=511;i++)
        {
            for(j=0;j<=511;j++)
            {
                Imagedata_channel[i][j][k]=(int)Imagedata[i][j][k];
            }
        }
    }
    //count the number of pixels for each intensity for each channel
    for(k=0;k<=2;k++)
    {
        for(i=0;i<=511;i++)
        {
            for(j=0;j<=511;j++)
            {
                numofpixels[k][Imagedata_channel[i][j][k]]=numofpixels[k][Imagedata_channel[i][j][k]]+1;
            }
        }
    }

    //transfer function
    for(k=0;k<=2;k++)
    {
        for(i=0;i<=255;i++)
        {
            sum=0;
            for(j=0;j<=i;j++)
            {
                sum=sum+numofpixels[k][j];
            }
            sumofpixels[k][i]=sum;
        }
    }

    //assign new intensity for each pixel
    for(chl=0;chl<=2;chl++)
    {
        for(k=0;k<=255;k++)
        {
            for(i=0;i<=511;i++)
            {
                for(j=0;j<=511;j++)
                {
                    if(Imagedata_channel[i][j][chl]==k)
                    {
                        Imagedata[i][j][chl]=(unsigned char)(255*sumofpixels[chl][k]/(size*size));
                    }
                }
            }
        }
    }
/*
	//out put the .txt file
	for(i=0;i<=255;i++)
	{
		cout<<numofpixels[1][i]<<endl;
	}
*/
	//plot the transfer function
    float y[3][256];
    int l;
	for(l=0;l<=2;l++)
	{
		for(k=0;k<=255;k++)
		{
			y[l][k]=sumofpixels[l][k]/262144;
		}

	}
	for(i=0;i<=255;i++)
	{
		cout<<y[0][i]<<endl;
	}

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(Imagedata, sizeof(unsigned char), size*size*3, file);
    fclose(file);

    return 0;
}
